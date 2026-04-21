#pragma once
#include "raylib.h"
#include <vector>
#include "Snake.h"
using namespace std;

class Food
{
private:
    float x;
    float z;

    float animTime; // NEW

public:
    Food();

    float GetX() const;
    float GetZ() const;

    void SetPosition(float newX, float newZ);

    void Respawn(int boardWidth, int boardHeight, const vector<Snake::Segment>& snakeBody);
    void Reset(int boardWidth, int boardHeight, const vector<Snake::Segment>& snakeBody);

    void Update(); // NEW
    void Draw(int boardWidth, int boardHeight) const;
};