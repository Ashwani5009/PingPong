#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 900
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_BACKGROUND 0xefefefef

struct racket {
  double x;
  double y;
  double w;
  double h;
};

struct Circle {
  double x;
  double y;
  double r;
  double x_v;
  double y_v;
};

void fillBall(SDL_Surface *surface, struct Circle *ball, Uint32 color) {
  double radiusSquared = pow(ball->r, 2);
  for (double x = ball->x - ball->r; x < ball->x + ball->r; x++) {
    for (double y = ball->y - ball->r; y < ball->y + ball->r; y++) {
      double distSquared = pow(x - ball->x, 2) + pow(y - ball->y, 2);
      if (distSquared < radiusSquared) {
        SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
        SDL_FillRect(surface, &pixel, color);
      }
    }
  }
}

void fillRacket(SDL_Surface *surface, struct racket *rectangle, Uint32 color) {
  for (double x = rectangle->x - rectangle->w; x < rectangle->x + rectangle->w;
       x++) {
    for (double y = rectangle->y - rectangle->h;
         y < rectangle->y + rectangle->h; y++) {
      SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
      SDL_FillRect(surface, &pixel, color);
    }
  }
}

int hitPlayer(struct Circle *ball, struct racket player, int flag) {
  double radiusSquared = pow(ball->r, 2);
  for (double a = player.y - player.h; a <= player.y + player.h; a++) {
    double b = 0;
    if (flag)
      b = player.x - player.w;
    else
      b = player.x + player.w;
    double distSquared = pow(a - ball->y, 2) + pow(b - ball->x, 2);
    if (distSquared < radiusSquared) {
      return 1;
    }
  }
  return 0;
}

int hitWindow(struct Circle ball, int flag) {
  double radiusSquared = pow(ball.r, 2);
  for (double a = 0; a <= WIDTH; a++) {
    double b = 0;
    if (flag)
      b = HEIGHT;
    double distSquared = pow(a - ball.x, 2) + pow(b - ball.y, 2);
    if (distSquared < radiusSquared) {
      return 1;
    }
  }
  return 0;
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("Ping Pong", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  struct racket player1 = {40, 150, 10, 100};
  struct racket player2 = {1160, 150, 10, 100};
  struct racket midLine = {600, 0, 10, HEIGHT};
  struct Circle ball = {85, 145, 30, 5, 5};
  SDL_Rect eraseRect = {0, 0, WIDTH, HEIGHT};
  int simulation_running = 1;
  int speed = 10;
  double angle = 2.3;
  int yPlayer1 = 0;
  int yPlayer2 = 0;
  SDL_Event event;
  while (simulation_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        simulation_running = 0;
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_DOWN:
          yPlayer2 = speed;
          break;
        case SDLK_UP:
          yPlayer2 = -speed;
          break;
        case SDLK_s:
          yPlayer1 = speed;
          break;
        case SDLK_w:
          yPlayer1 = -speed;
          break;
        default:
          break;
        }
        break;

      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_DOWN:
          if (yPlayer2 > 0) {
            yPlayer2 = 0;
          }
          break;
        case SDLK_UP:
          if (yPlayer2 < 0) {
            yPlayer2 = 0;
          }
          break;
        case SDLK_s:
          if (yPlayer1 > 0) {
            yPlayer1 = 0;
          }
          break;
        case SDLK_w:
          if (yPlayer1 < 0) {
            yPlayer1 = 0;
          }
          break;
        default:
          break;
        }
        break;

      default:
        break;
      }
    }
    if (player2.y + yPlayer2 - player2.h >= 0 &&
        player2.y + yPlayer2 + player2.h < HEIGHT) {
      player2.y += yPlayer2;
    }
    if (player1.y + yPlayer1 - player1.h >= 0 &&
      player1.y + yPlayer1 + player1.h < HEIGHT) {
      player1.y += yPlayer1;
    }
    SDL_FillRect(surface, &eraseRect, COLOR_BLACK);
    fillRacket(surface, &midLine, COLOR_WHITE);
    fillRacket(surface, &player1, COLOR_WHITE);
    fillRacket(surface, &player2, COLOR_WHITE);
    fillBall(surface, &ball, COLOR_WHITE);
    ball.x += ball.x_v * cos(angle);
    ball.y += ball.y_v * sin(angle);
    if (hitWindow(ball, 1) || hitWindow(ball, 0)) {
      angle = -angle;
    }
    if (hitPlayer(&ball, player1, 1)) {
      ball.x_v = -ball.x_v;
      angle = -angle;
    }
    if (hitPlayer(&ball, player2, 0)) {
      ball.x_v = -ball.x_v;
      angle = -angle;
    }
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
  }
}
