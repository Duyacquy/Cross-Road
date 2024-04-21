#include "Player.h"

Player::Player() {
    x = GM::SCREEN_WIDTH / 2 - GM::LAND_SIZE / 2;
    y = GM::SCREEN_HEIGHT - GM::LAND_SIZE * 3 + 5;
    direction = 0;
}

void Player::moveLeft() {
    x -= GM::LAND_SIZE;
    direction = 3;
}

void Player::moveRight() {
    x += GM::LAND_SIZE;
    direction = 1;
}

void Player::moveUp() {
    GM::Score++;
    if (y > 0) y -= GM::LAND_SIZE;
    if (GM::Score > GM::MaxScore) {
        GM::MaxScore = GM::Score;
        std::string scoreStr = std::to_string(GM::MaxScore);
        scoreSurface = TTF_RenderText_Solid(titleFont, scoreStr.c_str(), scoreColor);
        scoreTexture = SDL_CreateTextureFromSurface(GM::renderer, scoreSurface);
    }
    direction = 0;
}

void Player::moveDown() {
    y += GM::LAND_SIZE;
    GM::Score--;
    direction = 2;
}

void Player::moveOnWood(Wood* wood) {
    if (wood->movingLeft) {
        x -= wood->speed;
    }
    else {
        x += wood->speed;
    }
}

void Player::render() {
    if(!surface) surface = IMG_Load("./src/image/frog.png");
    if (!texture) texture = SDL_CreateTextureFromSurface(GM::renderer, surface);
    if (!jumpSound) jumpSound = Mix_LoadWAV("./src/sound/soundEffect/jumpSound.wav");
    if (!scoreTexture) {
        scoreColor = { 255, 255, 255 };
        titleFont = TTF_OpenFont("./src/font/Bebas_Neue/BebasNeue-Regular.ttf", 20);
        scoreSurface = TTF_RenderText_Solid(titleFont, "0", scoreColor);
        scoreTexture = SDL_CreateTextureFromSurface(GM::renderer, scoreSurface);
    }

    if (isJumping && jumpFrameTimeCounter >= jumpFrameTime) {
        if (isPlayJumpSound) {
            Mix_PlayChannel(-1, jumpSound, 0);
            isPlayJumpSound = false;
        }
        jumpFrameTimeCounter = 0;
        spriteIndex++;
        if (spriteIndex >= 7) {
            spriteIndex = 0;
            isJumping = false;
        }
    }

    SDL_Rect frogSrcRect;
    switch (spriteIndex) {
        case 0:
            frogSrcRect = { 11, 21, 100, 120 };
            break;
        case 1:
            frogSrcRect = { spriteIndex * 867 / 7, 23, 867 / 7, 120 };
            break;
        case 2:
            frogSrcRect = { spriteIndex * 867 / 7, 12, 867 / 7, 141 };
            break;
        case 3:
            frogSrcRect = { spriteIndex * 867 / 7, 7, 867 / 7, 153 };
            break;
        case 4:
            frogSrcRect = { spriteIndex * 867 / 7, 0, 867 / 7, 165 };
            break;
        case 5:
            frogSrcRect = { spriteIndex * 867 / 7, 9, 867 / 7, 150 };
            break;
        case 6:
            frogSrcRect = { spriteIndex * 867 / 7, 22, 867 / 7, 121 };
            break;
    }
    SDL_Rect frogDsRect = { x, y, GM::PLAYER_SIZE, GM::PLAYER_SIZE };
    switch (direction) {
        case 0: 
            SDL_RenderCopyEx(GM::renderer, texture, &frogSrcRect, &frogDsRect, 0, nullptr, SDL_FLIP_NONE);
            SDL_RenderPresent(GM::renderer);
            break;
        case 1: 
            SDL_RenderCopyEx(GM::renderer, texture, &frogSrcRect, &frogDsRect, 90, nullptr, SDL_FLIP_NONE);
            SDL_RenderPresent(GM::renderer);
            break;
        case 2:
            SDL_RenderCopyEx(GM::renderer, texture, &frogSrcRect, &frogDsRect, 180, nullptr, SDL_FLIP_NONE);
            SDL_RenderPresent(GM::renderer);
            break;
        case 3:
            SDL_RenderCopyEx(GM::renderer, texture, &frogSrcRect, &frogDsRect, 270, nullptr, SDL_FLIP_NONE);
            SDL_RenderPresent(GM::renderer);
            break;
    }
}
