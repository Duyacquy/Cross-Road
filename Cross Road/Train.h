#ifndef TRAIN_H
#define TRAIN_H
#include <vector>

#include "GameManager.h"

class Train {
public:
    int x;
    int y;
    bool movingLeft;
    int typeCar;
    int speed;

    Train(int startX, int startY, bool left, int typeTrain, int trainSpeed);

    void move();
};

#endif // TRAIN_H
