#include<SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#define WIDTH 1200
#define HEIGHT 900
#define COLOR_WHITE 0xffffffff

int main(){
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("Ping Pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
  
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_Rect player1 = (SDL_Rect) {40,40,20,200};
  SDL_Rect player2 = (SDL_Rect) {1140,40,20,200};
  SDL_Rect midLine = (SDL_Rect) {600,0,20,HEIGHT};
  int simulation_running = 1;
  int speed = 1;
  SDL_Event event;
  while(simulation_running){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        simulation_running = 0;
      }
    }
    SDL_FillRect(surface,&midLine,COLOR_WHITE);
    SDL_FillRect(surface,&player1,COLOR_WHITE);
    SDL_FillRect(surface,&player2,COLOR_WHITE);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
  }
}
