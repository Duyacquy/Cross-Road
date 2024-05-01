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
        if (typeCar == 3) {
            if (x < -110 * 6) x = GM::SCREEN_WIDTH;
        }
        else {
            if (x < -GM::TRAIN_WIDTH * 6) x = GM::SCREEN_WIDTH;
        }
            
    }
    else {
        x += speed;
        if (typeCar == 3) {
            if (x > GM::SCREEN_WIDTH * 2) x = -110;
        } else {
            if (x > GM::SCREEN_WIDTH * 2) x = -GM::TRAIN_WIDTH;
        }
    }
}
