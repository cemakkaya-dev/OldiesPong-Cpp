#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <cstdlib>
#include <string>

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  srand(SDL_GetTicks());

  SDL_Window *window =
      SDL_CreateWindow("Ping Pong", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 32);

  float leftY = 250.0f;
  float rightY = 250.0f;
  float ballX = 390.0f;
  float ballY = 290.0f;

  const int leftX = 30, rightX = 750;
  const int racketW = 20, racketH = 100, ballSize = 10;

  const float racketSpeed = 300.0f;
  float ballVX = 0.0f;
  float ballVY = 0.0f;

  bool ballWaiting = true;
  float waitTimer = 2.0f;

  int leftScore = 0;
  int rightScore = 0;
  bool gameOver = false;

  bool running = true;
  SDL_Event event;
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  Uint64 prevTime = SDL_GetPerformanceCounter();

  while (running) {

    // ---- DELTA TIME ----
    Uint64 currTime = SDL_GetPerformanceCounter();
    float dt =
        (float)(currTime - prevTime) / (float)SDL_GetPerformanceFrequency();
    prevTime = currTime;
    if (dt > 0.05f)
      dt = 0.05f;

    // ---- INPUT ----
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = false;
    }

    // ---- UPDATE ----
    if (!gameOver) {

      if (keys[SDL_SCANCODE_W] && leftY > 0)
        leftY -= racketSpeed * dt;
      if (keys[SDL_SCANCODE_S] && leftY < 500)
        leftY += racketSpeed * dt;

      if (keys[SDL_SCANCODE_UP] && rightY > 0)
        rightY -= racketSpeed * dt;
      if (keys[SDL_SCANCODE_DOWN] && rightY < 500)
        rightY += racketSpeed * dt;

      if (ballWaiting) {
        waitTimer -= dt;
        if (waitTimer <= 0.0f) {
          ballWaiting = false;
          float vx = 450.0f;
          float vy = 250.0f + (rand() % 150);
          ballVX = (rand() % 2 == 0) ? vx : -vx;
          ballVY = (rand() % 2 == 0) ? vy : -vy;
        }
      } else {
        ballX += ballVX * dt;
        ballY += ballVY * dt;

        if (ballX < 0) {
          rightScore++;
          if (rightScore >= 5)
            gameOver = true;
          ballX = 390.0f;
          ballY = 290.0f;
          ballWaiting = true;
          waitTimer = 2.0f;
        }

        if (ballX + ballSize > 800) {
          leftScore++;
          if (leftScore >= 5)
            gameOver = true;
          ballX = 390.0f;
          ballY = 290.0f;
          ballWaiting = true;
          waitTimer = 2.0f;
        }

        if (ballY < 0) {
          ballY = 0;
          ballVY = std::fabs(ballVY);
        }
        if (ballY + ballSize > 600) {
          ballY = 600 - ballSize;
          ballVY = -std::fabs(ballVY);
        }

        SDL_Rect ballRect = {(int)ballX, (int)ballY, ballSize, ballSize};
        SDL_Rect leftRect = {leftX, (int)leftY, racketW, racketH};
        SDL_Rect rightRect = {rightX, (int)rightY, racketW, racketH};

        if (SDL_HasIntersection(&ballRect, &leftRect))
          ballVX = std::fabs(ballVX);

        if (SDL_HasIntersection(&ballRect, &rightRect))
          ballVX = -std::fabs(ballVX);
      }
    }

    // ---- RENDERER ----
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect leftRect = {leftX, (int)leftY, racketW, racketH};
    SDL_Rect rightRect = {rightX, (int)rightY, racketW, racketH};
    SDL_Rect ballRect = {(int)ballX, (int)ballY, ballSize, ballSize};
    SDL_RenderFillRect(renderer, &leftRect);
    SDL_RenderFillRect(renderer, &rightRect);
    SDL_RenderFillRect(renderer, &ballRect);

    // ---- Skor Yazısı ----
    SDL_Color white = {255, 255, 255, 255};
    std::string scoreText =
        std::to_string(leftScore) + "  -  " + std::to_string(rightScore);
    SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {400 - surface->w / 2, 20, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // ---- Kazanma Yazısı ----
    if (gameOver) {
      std::string winText =
          (leftScore >= 5) ? "Left Player Wins!" : "Right Player Wins!";
      SDL_Surface *ws = TTF_RenderText_Solid(font, winText.c_str(), white);
      SDL_Texture *wt = SDL_CreateTextureFromSurface(renderer, ws);
      SDL_Rect wr = {400 - ws->w / 2, 280, ws->w, ws->h};
      SDL_RenderCopy(renderer, wt, NULL, &wr);
      SDL_FreeSurface(ws);
      SDL_DestroyTexture(wt);
    }

    SDL_RenderPresent(renderer);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
