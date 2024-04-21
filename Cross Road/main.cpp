#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "GameManager.h"
#include "Player.h"
#include "Train.h"
#include "Wood.h"

int mapOfGame[25][25];

Player* player = new Player();

std::vector<Wood*> woods;
std::vector<Train*> trains;

bool quit = false;
bool playGame = false;

int countDirection = 0;
int countRoadSprite = 0;
int countRiverSprite = 0;
int countFrogSprite = 0;

int moved = 0;

SDL_Surface* endGameSurface = IMG_Load("./src/image/alert-box.png");
SDL_Surface* grassSurface = IMG_Load("./src/image/grass.png");
SDL_Surface* roadSurface = IMG_Load("./src/image/road.png");
SDL_Surface* riverSurface = IMG_Load("./src/image/water.png");
SDL_Surface* woodSurface = IMG_Load("./src/image/wood.png");
SDL_Surface* car1Surface = IMG_Load("./src/image/car1.png");
SDL_Surface* car2Surface = IMG_Load("./src/image/car2.png");
SDL_Surface* car3Surface = IMG_Load("./src/image/car3.png");
SDL_Surface* playAgainSurface = IMG_Load("./src/image/playagain.png");

SDL_Texture* endGameTexture;
SDL_Texture* grassTexture;
SDL_Texture* roadTexture;
SDL_Texture* riverTexture;
SDL_Texture* woodTexture;
SDL_Texture* car1Texture;
SDL_Texture* car2Texture;
SDL_Texture* car3Texture;
SDL_Texture* playAgainTexture;

SDL_Rect scoreDsRect = { 10, 10, 50, 70 };
SDL_Rect roadSrcRect = { 0, 0, 320, 180 };
SDL_Rect grassSrcRect = { 0, 0, 320, 320 };
SDL_Rect riverSrcRect = { 0, 0, 446, 192 };
SDL_Rect woodSrcRect = { 0, 0, 311, 64 };
SDL_Rect car1SrcRect = { 0, 0, 1280, 687 };
SDL_Rect car1DsRect;
SDL_Rect car2SrcRect = { 0, 0, 2400, 1190 };
SDL_Rect car2DsRect;
SDL_Rect car3SrcRect = { 0, 0, 448, 134 };
SDL_Rect car3DsRect;
SDL_Rect frogSrcRect = { 30, 50, 110, 120 };

void renderMapStart() {
    for (int j = 0; j < 13; j++) {
        mapOfGame[0][j] = 1;
        mapOfGame[1][j] = 1;
        mapOfGame[2][j] = 1;
    }
    for (int i = 3; i < 17; i++) {
        int randNum = rand() % 100;
        for (int j = 0; j < 13; j++) {
            if (randNum < 20) {
                mapOfGame[i][j] = 1;
            }
            else if (randNum < 65) {
                mapOfGame[i][j] = 2;
            }
            else {
                mapOfGame[i][j] = 3;
            }
        }
    }
}

void renderMap() {
    moved++;
    for (auto& train : trains) {
        train->y++;
    }
    for (auto& wood : woods) {
        wood->y++;
    }
    player->y++;
    if (moved == GM::LAND_SIZE) {
        moved = 0;
        if (mapOfGame[0][0] % 10 == 2) {
            trains.erase(trains.begin());
            trains.erase(trains.begin());
        }
        if (mapOfGame[0][0] % 10 == 3) {
            woods.erase(woods.begin());
            woods.erase(woods.begin());
            woods.erase(woods.begin());
        }
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 13; j++) mapOfGame[i][j] = mapOfGame[i + 1][j];
        }
        int randNumMap = rand() % 100;
        for (int j = 0; j < 13; j++) {
            if (randNumMap < 20) mapOfGame[16][j] = 1;
            else if (randNumMap < 65) mapOfGame[16][j] = 2;
            else mapOfGame[16][j] = 3;
        }
    }
}

void handleInput(SDL_Event &e) {
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            player->moveLeft();
            player->isJumping = true;
            player->isPlayJumpSound = true;
            break;
        case SDLK_RIGHT:
            player->moveRight();
            player->isJumping = true;
            player->isPlayJumpSound = true;
            break;
        case SDLK_UP:
            player->moveUp();
            player->isJumping = true;
            player->isPlayJumpSound = true;
            break;
        case SDLK_DOWN:
            player->moveDown();
            player->isJumping = true;
            player->isPlayJumpSound = true;
            break;
        }
    }
}

bool checkCollisionWithWindow(int x, int y) {
    if (x < 0 || x > GM::SCREEN_WIDTH - GM::PLAYER_SIZE) return true;
    if (y > (690 - GM::PLAYER_SIZE)) return true;
    return false;
}

void spawnTrain(int y, int x, int trainStartX, bool left, int typeTrain, int trainSpeed) {
    int startY = 690 - (y + 1) * 46 + 5;
    Train* newTrain;
    if (typeTrain == 3) newTrain = new Train(left ? (GM::SCREEN_WIDTH - trainStartX) + x * 598 : -110 + trainStartX - x * 598, startY, left, typeTrain, trainSpeed);
    else newTrain = new Train(left ? (GM::SCREEN_WIDTH - trainStartX) + x * 598 : -GM::TRAIN_WIDTH + trainStartX - x * 598, startY, left, typeTrain, trainSpeed);
    trains.push_back(newTrain);
}

void spawnWood(int y, int x, int woodStartX, bool left, int woodSpeed, int woodWidth) {
    int startY = 690 - (y + 1) * 46 + 5;
    Wood* newWood = new Wood(left ? GM::SCREEN_WIDTH - woodStartX + x * 230 : -woodWidth + woodStartX - x * 230, startY, left, woodSpeed);
    woods.push_back(newWood);
}

void renderObject() {
    for (int i = 0; i < 17; i++) {
        if (mapOfGame[i][0] == 2) {
            int numTrain = 2;
            bool left = rand() % 2 == 0;
            int typeTrain = rand() % 3 + 1;
            int trainSpeed = rand() % 2 + 1;
            int trainStartX = rand() % 401 + 100;
            for (int j = 0; j < numTrain; j++) {
                spawnTrain(i, j, trainStartX, left, typeTrain, trainSpeed);
            }
            mapOfGame[i][0] += 10;
        }
        else if (mapOfGame[i][0] == 3) {
            bool left;
            int numWood = 3;
            if (countDirection == 0) {
                left = true;
                countDirection++;
            }
            else {
                left = false;
                countDirection = 0;
            }
            int woodSpeed = rand() % 2 + 1;
            int woodStartX = rand() % 301 + 100;
            for (int j = 0; j < numWood; j++) {
                spawnWood(i, j, woodStartX, left, woodSpeed, GM::WOOD_WIDTH);
            }
            mapOfGame[i][0] += 10;
        }
    }
}

void update() {
    player->jumpFrameTimeCounter += 1000.0 / 70.0;

    for (auto& train : trains) {
        train->move();
    }
    for (auto& wood : woods) {
        wood->move();
    }
}

void render() {
    if (!grassTexture) {
        grassTexture = SDL_CreateTextureFromSurface(GM::renderer, grassSurface);
    }
    if (!roadTexture) {
        roadTexture = SDL_CreateTextureFromSurface(GM::renderer, roadSurface);
    }
    if (!riverTexture) {
        riverTexture = SDL_CreateTextureFromSurface(GM::renderer, riverSurface);
    }
    if (!woodTexture) {
        woodTexture = SDL_CreateTextureFromSurface(GM::renderer, woodSurface);
    }
    if (!car1Texture) {
        car1Texture = SDL_CreateTextureFromSurface(GM::renderer, car1Surface);
    }
    if (!car2Texture) {
        car2Texture = SDL_CreateTextureFromSurface(GM::renderer, car2Surface);
    }
    if (!car3Texture) {
        car3Texture = SDL_CreateTextureFromSurface(GM::renderer, car3Surface);
    }

    SDL_Rect frogDsRect = { player->x, player->y, GM::PLAYER_SIZE, GM::PLAYER_SIZE };

    // Vẽ địa hình
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 13; j++) {
            if (mapOfGame[i][j] % 10 == 1) {
                //Vẽ ô cỏ
                SDL_Rect grassDsRect = { j * 46, 690 - (i + 1) * 46 + moved, GM::LAND_SIZE, GM::LAND_SIZE };
                SDL_RenderCopy(GM::renderer, grassTexture, &grassSrcRect, &grassDsRect);
            }
            else if (mapOfGame[i][j] % 10 == 2) {
                // Vẽ ô đường
                SDL_Rect roadDsRect = { j * 46, 690 - (i + 1) * 46 + moved, GM::LAND_SIZE, GM::LAND_SIZE };
                SDL_RenderCopy(GM::renderer, roadTexture, &roadSrcRect, &roadDsRect);
            }
            else if (mapOfGame[i][j] % 10 == 3) {
                // Vẽ ô sông
                SDL_Rect riverDsRect = { j * 46, 690 - (i + 1) * 46 + moved, GM::LAND_SIZE, GM::LAND_SIZE };
                SDL_RenderCopy(GM::renderer, riverTexture, &riverSrcRect, &riverDsRect);

                if (SDL_HasIntersection(&frogDsRect, &riverDsRect)) {
                    bool onWood = false;
                    for (auto& wood : woods) {
                        SDL_Rect woodDsRect = { wood->x, wood->y, GM::WOOD_WIDTH, GM::WOOD_HEIGHT };
                        SDL_RenderCopy(GM::renderer, woodTexture, &woodSrcRect, &woodDsRect);
                            
                        if (SDL_HasIntersection(&frogDsRect, &woodDsRect)) {
                            onWood = true;
                            break;
                        }
                    }
                    if (!onWood) {
                        Mix_Chunk* lossSound = Mix_LoadWAV("./src/sound/soundEffect/gameover.wav");
                        Mix_PlayChannel(-1, lossSound, 0);
                        GM::gameState = GM::GT_End;
                        break;
                    }
                }
            }
        }
    }

    // Vẽ các đoàn tàu
    for (auto& train : trains) {
        switch(train->typeCar) {
            case 1:
                car1DsRect = { train->x, train->y, GM::TRAIN_WIDTH, GM::TRAIN_HEIGHT };
                if (train->movingLeft) SDL_RenderCopyEx(GM::renderer, car1Texture, &car1SrcRect, &car1DsRect, 180, nullptr, SDL_FLIP_NONE);
                else SDL_RenderCopy(GM::renderer, car1Texture, &car1SrcRect, &car1DsRect);
                if (SDL_HasIntersection(&frogDsRect, &car1DsRect)) {
                    Mix_Chunk* lossSound = Mix_LoadWAV("./src/sound/soundEffect/gameover.wav");
                    Mix_PlayChannel(-1, lossSound, 0);
                    GM::gameState = GM::GT_End;
                    break;
                }
                break;
            case 2:
                car2DsRect = { train->x, train->y, GM::TRAIN_WIDTH, GM::TRAIN_HEIGHT };
                if (train->movingLeft) SDL_RenderCopyEx(GM::renderer, car2Texture, &car2SrcRect, &car2DsRect, 180, nullptr, SDL_FLIP_NONE);
                else SDL_RenderCopy(GM::renderer, car2Texture, &car2SrcRect, &car2DsRect);
                if (SDL_HasIntersection(&frogDsRect, &car2DsRect)) {
                    Mix_Chunk* lossSound = Mix_LoadWAV("./src/sound/soundEffect/gameover.wav");
                    Mix_PlayChannel(-1, lossSound, 0);
                    GM::gameState = GM::GT_End;
                    break;
                }
                break;
            case 3:
                car3DsRect = { train->x, train->y, 110, GM::TRAIN_HEIGHT };
                if (train->movingLeft) SDL_RenderCopyEx(GM::renderer, car3Texture, &car3SrcRect, &car3DsRect, 180, nullptr, SDL_FLIP_NONE);
                else SDL_RenderCopy(GM::renderer, car3Texture, &car3SrcRect, &car3DsRect);
                if (SDL_HasIntersection(&frogDsRect, &car3DsRect)) {
                    Mix_Chunk* lossSound = Mix_LoadWAV("./src/sound/soundEffect/gameover.wav");
                    Mix_PlayChannel(-1, lossSound, 0);
                    GM::gameState = GM::GT_End;
                    break;
                }
                break;
        }
    }

    for (auto& wood : woods) {
        SDL_Rect woodDsRect = { wood->x, wood->y, GM::WOOD_WIDTH, GM::WOOD_HEIGHT };
        SDL_RenderCopy(GM::renderer, woodTexture, &woodSrcRect, &woodDsRect);

        if (SDL_HasIntersection(&frogDsRect, &woodDsRect)) {
            player->moveOnWood(wood);
        }

    }

    if (checkCollisionWithWindow(player->x, player->y)) {
        GM::gameState = GM::GT_End;
        Mix_Chunk* lossSound = Mix_LoadWAV("./src/sound/soundEffect/gameover.wav");
        Mix_PlayChannel(-1, lossSound, 0);
    }

    player->render();
    SDL_RenderCopy(GM::renderer, player->scoreTexture, nullptr, &scoreDsRect);

    SDL_RenderPresent(GM::renderer);
}

int main(int argc, char* argv[]) {
    GM::initSDL();

    srand(time(NULL));

    const int SCREEN_TICK_PER_FRAME = 1000 / 100;

    GM::start3s = SDL_GetTicks();

    // Ảnh lúc bắt đầu game
    SDL_Surface* startSurface = IMG_Load("./src/image/start.jpg");
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(GM::renderer, startSurface);
    SDL_FreeSurface(startSurface);
    SDL_Rect startSrcRect = { 0, 0, 1712, 1800 };
    SDL_Rect startDsRect = { 0, 0, 598, 690 };

    // Tên game lúc bắt đầu game
    TTF_Font* titleFont = TTF_OpenFont("./src/font/Bebas_Neue/BebasNeue-Regular.ttf", 48);
    SDL_Color titleColor = { 255, 0, 0 };
    SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "Crossy Road", titleColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(GM::renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    SDL_Rect titleDsRect = { 65, 20, 470, 180 };

    // Phát nhạc game 
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music* backgroundMusic = Mix_LoadMUS("./src/sound/backgroundMusic/POL-follow-me-short.wav");

    // Bật/tắt nút âm thanh
    SDL_Surface* onMusicSurface = IMG_Load("./src/image/musicon.png");
    SDL_Texture* onMusicTexture = SDL_CreateTextureFromSurface(GM::renderer, onMusicSurface);
    SDL_FreeSurface(onMusicSurface);
    SDL_Rect onMusicSrcRect = { 0, 0, 348, 348 };
    SDL_Rect onMusicDsRect = { 450, 10, 100, 100 };

    SDL_Surface* offMusicSurface = IMG_Load("./src/image/musicoff.png");
    SDL_Texture* offMusicTexture = SDL_CreateTextureFromSurface(GM::renderer, offMusicSurface);
    SDL_FreeSurface(offMusicSurface);
    SDL_Rect offMusicSrcRect = { 0, 0, 348, 348 };
    SDL_Rect offMusicDsRect = { 450, 10, 100, 100 };

    // Tiếng click khi ấn nút play
    Mix_Chunk* clickToPlaySound = Mix_LoadWAV("./src/sound/soundEffect/mixkit-select-click-1109.wav");

    // Hiện ảnh lúc hết game
    SDL_Rect backgroundRect = { 0, 0, 598, 690 };
    SDL_Rect endGameSrcRect = { 0, 0, 1328, 1088 };
    SDL_Rect endGameDsRect = { 75, 145, 450, 400 };

    // Hiện chữ game over
    SDL_Color gameOverTextColor = { 255, 0, 0 };
    SDL_Surface* gameOverTextSurface = TTF_RenderText_Solid(titleFont, "Game Over", gameOverTextColor);
    SDL_Texture* gameOverTextTexture = SDL_CreateTextureFromSurface(GM::renderer, gameOverTextSurface);
    SDL_FreeSurface(gameOverTextSurface);
    SDL_Rect gameOverTextDsRect = { 217, 220, 170, 80 };

    // Hiện số điểm khi kết thúc game
    SDL_Color scoreEndGameColor = { 255, 215, 0 };
    SDL_Surface* scoreEndGameSurface = nullptr;
    SDL_Texture* scoreEndGameTexture = nullptr;
    SDL_Rect scoreEndGameDsRect;

    // Hiện nút chơi lại
    SDL_Rect playAgainSrcRect = { 0, 0, 375, 250 };
    SDL_Rect playAgainDsRect = { 260, 395, 72, 48 };

    renderMapStart();
  
    while(!quit) {
        switch (GM::gameState) {
        case GM::GT_Start:
            SDL_RenderCopy(GM::renderer, startTexture, &startSrcRect, &startDsRect);
            SDL_RenderCopy(GM::renderer, titleTexture, nullptr, &titleDsRect);
            SDL_RenderPresent(GM::renderer);
            Mix_PlayMusic(backgroundMusic, -1);
            while (!playGame) {
                SDL_Event clickToPlay;
                while (SDL_PollEvent(&clickToPlay) != 0) {
                    if (clickToPlay.type == SDL_MOUSEBUTTONDOWN) {
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);
                        if (mouseX >= 225 && mouseX <= 375 && mouseY >= 535 && mouseY <= 595) {
                            Mix_PlayChannel(-1, clickToPlaySound, 0);
                            SDL_DestroyTexture(startTexture);
                            TTF_CloseFont(titleFont);
                            playGame = true;
                            GM::gameState = GM::GT_Play;
                        }
                    }
                    
                }
            }
            break;
        case GM::GT_Play:
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0) {
                handleInput(e);
            }
            GM::startTime = SDL_GetTicks();
            render();
            renderObject();
            update();

            GM::endTime = SDL_GetTicks();
            GM::end3s = SDL_GetTicks();

            // Kiểm soát thời gian giữa các khung hình
            if (GM::endTime - GM::startTime < SCREEN_TICK_PER_FRAME) {
                SDL_Delay(SCREEN_TICK_PER_FRAME - GM::endTime + GM::startTime);
            }

            if (GM::end3s - GM::start3s >= GM::renderSpeed) {
                renderMap();
                GM::start3s = SDL_GetTicks();
            }
            break;
        case GM::GT_End:
            if (!endGameTexture) endGameTexture = SDL_CreateTextureFromSurface(GM::renderer, endGameSurface);
            if (!playAgainTexture) playAgainTexture = SDL_CreateTextureFromSurface(GM::renderer, playAgainSurface);
            if (scoreEndGameTexture == nullptr) {
                SDL_SetRenderDrawColor(GM::renderer, 192, 192, 192, 50);
                SDL_RenderFillRect(GM::renderer, &backgroundRect);
                std::string scoreStr = std::to_string(GM::MaxScore);
                std::string scoreEndGameStr = "Score: " + scoreStr;
                switch ((int)scoreStr.length()) {
                    case 1:
                        scoreEndGameDsRect = { 226, 305, 150, 60 };
                        break;
                    case 2:
                        scoreEndGameDsRect = { 215, 305, 170, 60 };
                        break;
                    case 3: 
                        scoreEndGameDsRect = { 205, 305, 190, 60 };
                        break;
                    case 4: 
                        scoreEndGameDsRect = { 195, 305, 210, 60 };
                        break;
                    default:
                        scoreEndGameDsRect = { 185, 305, 230, 60 };
                        break;
                }
                TTF_Font* scoreEndGameFont = TTF_OpenFont("./src/font/Bebas_Neue/BebasNeue-Regular.ttf", 25);
                scoreEndGameSurface = TTF_RenderText_Solid(scoreEndGameFont, scoreEndGameStr.c_str(), scoreEndGameColor);
                scoreEndGameTexture = SDL_CreateTextureFromSurface(GM::renderer, scoreEndGameSurface);
            }

            SDL_RenderCopy(GM::renderer, endGameTexture, &endGameSrcRect, &endGameDsRect);
            SDL_RenderCopy(GM::renderer, gameOverTextTexture, nullptr, &gameOverTextDsRect);
            SDL_RenderCopy(GM::renderer, scoreEndGameTexture, nullptr, &scoreEndGameDsRect);
            SDL_RenderCopy(GM::renderer, playAgainTexture, &playAgainSrcRect, &playAgainDsRect);
            SDL_RenderPresent(GM::renderer);

            SDL_Event clickToPlayAgain;
            while (SDL_PollEvent(&clickToPlayAgain) != 0) {
                if (clickToPlayAgain.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (mouseX >= 237 && mouseX <= 352 && mouseY >= 390 && mouseY <= 450) {
                        Mix_PlayChannel(-1, clickToPlaySound, 0);
                        GM::MaxScore = 0;
                        GM::Score = 0;
                        GM::renderSpeed = 40;
                        renderMapStart();
                        player->x = GM::SCREEN_WIDTH / 2 - GM::LAND_SIZE / 2;
                        player->y = GM::SCREEN_HEIGHT - GM::LAND_SIZE * 3 + 5;
                        player->direction = 0;
                        quit = false;
                        playGame = false;
                        countDirection = 0;
                        countRoadSprite = 0;
                        countRiverSprite = 0;
                        countFrogSprite = 0;
                        moved = 0;
                        woods.clear();
                        trains.clear();
                        scoreEndGameTexture = nullptr;
                        player->scoreTexture = nullptr;
                        GM::gameState = GM::GT_Play;
                    }
                }

            }
            break;
        }
    }
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(clickToPlaySound);
    SDL_FreeSurface(riverSurface);
    SDL_FreeSurface(woodSurface);
    SDL_FreeSurface(car2Surface);
    GM::closeSDL();
    return 0;
}
