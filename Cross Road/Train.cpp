#include "Train.h"
#include <vector>

Train::Train(int startX, int startY, bool left,int typeTrain, int trainSpeed) {
    x = startX;
    y = startY;
    movingLeft = left;
    typeCar = typeTrain;
    speed = trainSpeed;
}

void Train::move() {
    if (movingLeft) {
        x -= speed;
        if (x < -GM::TRAIN_WIDTH * 6) x = GM::SCREEN_WIDTH;
    }
    else {
        x += speed;
        if (x > GM::SCREEN_WIDTH * 2) x = -GM::TRAIN_WIDTH;
    }
}
