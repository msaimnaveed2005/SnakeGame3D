#pragma once
#include "raylib.h"
using namespace std;

class Snake
{
private:
    float x;
    float z;
    int direction;

public:
    Snake();

    void SetDirection(int newDirection);
    void Move();

    float GetX() const;
    float GetZ() const;

    void Draw() const;
};