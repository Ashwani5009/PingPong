#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#define WIDTH 1200
#define HEIGHT 900
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000

struct racket {
  double x;
  double y;
  double w;
  double h;
};

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

int main() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("Ping Pong", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  struct racket player1 = {40, 150, 10, 100};
  struct racket player2 = {1160, 150, 10, 100};
  struct racket midLine = {600, 0, 10, HEIGHT};
  SDL_Rect eraseRect = {0, 0, WIDTH, HEIGHT};
  fillRacket(surface, &player1, COLOR_WHITE);
  int simulation_running = 1;
  int speed = 5;
  double x = 0, y = 0;
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
          y = 3;
          break;
        case SDLK_UP:
          y = -3;
          break;
        default:
          break;
        }
        break;

      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_DOWN:
          if (y > 0) {
            y = 0;
          }
          break;
        case SDLK_UP:
          if (y < 0) {
            y = 0;
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
    if(player1.y + y - player1.h >= 0 && player1.y + y + player1.h < HEIGHT){
      player1.y += y;
    }
    SDL_FillRect(surface, &eraseRect, COLOR_BLACK);
    fillRacket(surface, &midLine, COLOR_WHITE);
    fillRacket(surface, &player1, COLOR_WHITE);
    fillRacket(surface, &player2, COLOR_WHITE);
    player2.y += speed;
    if (player2.y + player2.h > HEIGHT) {
      speed = -speed;
    }
    if (player2.y - player2.h < 0) {
      speed = -speed;
    }
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
  }
}
