#pragma once
#include "raylib.h"
#include <vector>
using namespace std;

class Snake
{
public:
    struct Segment
    {
        float x;
        float z;
    };

private:
    vector<Segment> body;
    int direction;

public:
    Snake();

    void Reset();

    void SetDirection(int newDirection);
    void Move();
    void Grow();

    float GetX() const;
    float GetZ() const;

    const vector<Segment>& GetBody() const;

    bool CheckWallCollision(int width, int height) const;
    bool CheckSelfCollision() const;

    void Draw(int boardWidth, int boardHeight) const;
};