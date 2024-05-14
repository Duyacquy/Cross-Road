#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class GameManager {
public:
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    static int PLAYER_SIZE;
    static int LAND_SIZE;
    static int TRAIN_WIDTH;
    static int TRAIN_HEIGHT;
    static int WOOD_HEIGHT;
    static int WOOD_WIDTH;
    static int Score;
    static int MaxScore;
    static int renderSpeed;
    static bool fastCar;
    static Uint32 startTime;
    static Uint32 endTime;
    static Uint32 start3s;
    static Uint32 end3s;

    static SDL_Window* window;
    static SDL_Renderer* renderer;

    static void initSDL();
    static void closeSDL();

    enum GameState {
        GT_Start, GT_Play, GT_End
    };

    static GameState gameState;

};

typedef GameManager GM;

#endif // GAMEMANAGER_H
