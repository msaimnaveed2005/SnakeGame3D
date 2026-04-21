#pragma once
#include "raylib.h"
using namespace std;

class Food
{
private:
    float x;
    float z;

public:
    Food();

    float GetX() const;
    float GetZ() const;

    void SetPosition(float newX, float newZ);
    void Respawn(int boardWidth, int boardHeight);

    void Draw(int boardWidth, int boardHeight) const;
};