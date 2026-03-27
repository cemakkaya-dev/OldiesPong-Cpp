#include "SDL2/SDL_events.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstddef>
#include <iostream>
#include <string>

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_Window *window =
      SDL_CreateWindow("Ping Pong", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 32);

  SDL_Rect leftRacket = {30, 250, 20, 100};
  SDL_Rect rightRacket = {750, 250, 20, 100};
  SDL_Rect ball = {390, 290, 10, 10};

  int racketSpeed = 5;
  int ballVelocityX = 9;
  int ballVelocityY = 6;
  int leftScore = 0;
  int rightScore = 0;
  bool gameOver = false;

  bool running = true;
  SDL_Event event;
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  while (running) {

    // ---- INPUT ----
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    // ---- UPDATE ----

    if (!gameOver) {

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

      if (ball.x < 0) {
        rightScore++;
        if (rightScore >= 5)
          gameOver = true;
        ball.x = 390;
        ball.y = 290;
      }

      if (ball.x > 800) {
        leftScore++;
        if (rightScore >= 5)
          gameOver = true;
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
    }

    // ---- RENDERER ----
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &leftRacket);
    SDL_RenderFillRect(renderer, &rightRacket);
    SDL_RenderFillRect(renderer, &ball);

    // ---- Writing Score Text ----
    SDL_Color white = {255, 255, 255, 255};
    std::string scoreText =
        std::to_string(leftScore) + "  -  " + std::to_string(rightScore);
    SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {400 - surface->w / 2, 20, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // ---- Winning Text ----
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
    SDL_Delay(16);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}