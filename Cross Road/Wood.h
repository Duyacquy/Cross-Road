#ifndef WOOD_H
#define WOOD_H
#include "GameManager.h"

class Wood {
public:
    int x;
    int y;
    bool movingLeft;
    int speed;

    Wood(int startX, int startY, bool left, int woodSpeed);

    void move();
};


#endif // WOOD_H
