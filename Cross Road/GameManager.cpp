#include "GameManager.h"

int GM::SCREEN_WIDTH = 598;
int GM::SCREEN_HEIGHT = 690;
int GM::PLAYER_SIZE = 36;
int GM::LAND_SIZE = 46;
int GM::TRAIN_WIDTH = 65;
int GM::TRAIN_HEIGHT = 36;
int GM::WOOD_HEIGHT = 36;
int GM::WOOD_WIDTH = 100;
int GM::Score = 0;
int GM::MaxScore = 0;
int GM::renderSpeed = 20;
Uint32 GM::startTime = 0;
Uint32 GM::endTime = 0;
Uint32 GM::start3s = 0;
Uint32 GM::end3s = 0;

SDL_Window* GM::window = NULL;
SDL_Renderer* GM::renderer = NULL;

GM::GameState GM::gameState = GM::GT_Start;

void GM::initSDL() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    GM::window = SDL_CreateWindow("Crossy Road", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GM::SCREEN_WIDTH, GM::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    GM::renderer = SDL_CreateRenderer(GM::window, -1, 0);
}

void GM::closeSDL() {
    SDL_DestroyRenderer(GM::renderer);
    SDL_DestroyWindow(GM::window);
    SDL_Quit();
}
