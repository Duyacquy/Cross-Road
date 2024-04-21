#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "GameManager.h"
#include "Wood.h"
#include <string>

class Player {
public:
    int x, y;
    int direction;

    int spriteIndex = 0;
    double jumpFrameTime = 20;
    double jumpFrameTimeCounter = jumpFrameTime;
    bool isFrogJumping = false;
    bool isJumping = false;
    bool isPlayJumpSound = false;

    TTF_Font* titleFont;
    SDL_Color scoreColor;
    SDL_Surface* scoreSurface;
    SDL_Surface* surface;
    SDL_Texture* scoreTexture;
    SDL_Texture* texture;
    Mix_Chunk* jumpSound;


    Player();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveOnWood(Wood* wood);
    void render();
};

#endif // PLAYER_H
