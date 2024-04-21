#include "Wood.h"

Wood::Wood(int startX, int startY, bool left, int woodSpeed) {
    x = startX;
    y = startY;
    movingLeft = left;
    speed = woodSpeed;
}

void Wood::move() {
    if (movingLeft) {
        x -= speed;
        if (x < -GM::WOOD_WIDTH)
            x = GM::SCREEN_WIDTH;
    }
    else {
        x += speed;
        if (x > GM::SCREEN_WIDTH)
            x = -GM::WOOD_WIDTH;
    }
}
