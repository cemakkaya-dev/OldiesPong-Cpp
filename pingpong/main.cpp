#include <SDL2/SDL.h>
#include <iostream>

int main() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("Ping Pong", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool running = true;
  SDL_Event event;

  SDL_Rect leftRacket = {30, 250, 20, 100};
  SDL_Rect rightRacket = {750, 250, 20, 100};
  SDL_Rect ball = {390, 290, 10, 10};

  int racketSpeed = 5;
  int ballVelocityX = 9;
  int ballVelocityY = 6;
  int leftPlayerScore = 0;
  int rightPlayerScore = 0;

  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  while (running) {

    // ---- INPUT ----
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    // ---- UPDATE ----

    if (keys[SDL_SCANCODE_W] && leftRacket.y > 0) {
      leftRacket.y -= racketSpeed;
    }

    if (keys[SDL_SCANCODE_S] && leftRacket.y < 500) {
      leftRacket.y += racketSpeed;
    }

    if (keys[SDL_SCANCODE_UP] && rightRacket.y > 0) {
      rightRacket.y -= racketSpeed;
    }

    if (keys[SDL_SCANCODE_DOWN] && rightRacket.y < 500) {
      rightRacket.y += racketSpeed;
    }

    ball.x += ballVelocityX;
    ball.y += ballVelocityY;

    if (ball.x > 790) {
      ++leftPlayerScore;
      ball.x = 390;
      ball.y = 290;
    }

    if (ball.x < 0) {
      ++rightPlayerScore;
      ball.x = 390;
      ball.y = 290;
    }

    if (ball.y > 590 || ball.y < 0) {
      ballVelocityY = -ballVelocityY;
    }

    if (SDL_HasIntersection(&ball, &leftRacket)) {
      ballVelocityX = -ballVelocityX;
    }

    if (SDL_HasIntersection(&ball, &rightRacket)) {
      ballVelocityX = -ballVelocityX;
    }

    // ---- RENDERER ----
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &leftRacket);
    SDL_RenderFillRect(renderer, &rightRacket);
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}