#pragma once
#include "raylib.h"
#include <vector>
using namespace std;

class Snake
{
private:
    struct Segment
    {
        float x;
        float z;
    };

    vector<Segment> body;
    int direction;

public:
    Snake();

    void SetDirection(int newDirection);
    void Move();
    void Grow();

    float GetX() const;
    float GetZ() const;

    bool CheckWallCollision(int width, int height) const;

    void Draw(int boardWidth, int boardHeight) const;
};