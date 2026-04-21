#include "Snake.h"

Snake::Snake()
{
    Reset();
}

void Snake::Reset()
{
    body.clear();
    body.push_back({ 10.0f, 10.0f });
    direction = 0;
}

void Snake::SetDirection(int newDirection)
{
    if (newDirection == 4 && direction != 3) direction = 4;
    if (newDirection == 3 && direction != 4) direction = 3;
    if (newDirection == 1 && direction != 2) direction = 1;
    if (newDirection == 2 && direction != 1) direction = 2;
}

void Snake::Move()
{
    for (int i = (int)body.size() - 1; i > 0; i--)
    {
        body[i] = body[i - 1];
    }

    if (direction == 4) body[0].x += 1;
    if (direction == 3) body[0].x -= 1;
    if (direction == 1) body[0].z -= 1;
    if (direction == 2) body[0].z += 1;
}

void Snake::Grow()
{
    body.push_back(body.back());
}

float Snake::GetX() const
{
    return body[0].x;
}

float Snake::GetZ() const
{
    return body[0].z;
}

const vector<Snake::Segment>& Snake::GetBody() const
{
    return body;
}

bool Snake::CheckWallCollision(int width, int height) const
{
    return (body[0].x < 0 || body[0].x >= width || body[0].z < 0 || body[0].z >= height);
}

bool Snake::CheckSelfCollision() const
{
    for (int i = 1; i < (int)body.size(); i++)
    {
        if (body[0].x == body[i].x && body[0].z == body[i].z)
            return true;
    }
    return false;
}

void Snake::Draw(int boardWidth, int boardHeight, bool premiumMode) const
{
    for (int i = 0; i < (int)body.size(); i++)
    {
        float worldX = body[i].x - boardWidth / 2.0f + 0.5f;
        float worldZ = body[i].z - boardHeight / 2.0f + 0.5f;

        if (premiumMode)
        {
            DrawCube({ worldX, 0.05f, worldZ }, 1.0f, 0.1f, 1.0f, Color{ 0, 0, 0, 95 });

            if (i == 0)
            {
                DrawCube({ worldX, 0.68f, worldZ }, 1.05f, 1.15f, 1.05f, Color{ 0, 235, 135, 255 });
                DrawCubeWires({ worldX, 0.68f, worldZ }, 1.08f, 1.18f, 1.08f, Color{ 200, 255, 225, 255 });
                DrawCube({ worldX, 1.18f, worldZ }, 0.35f, 0.10f, 0.35f, Color{ 160, 255, 210, 160 });
            }
            else
            {
                int fade = 185 - i * 5;
                if (fade < 85) fade = 85;

                DrawCube({ worldX, 0.60f, worldZ }, 0.92f, 1.0f, 0.92f, Color{ 0, (unsigned char)fade, 120, 255 });
            }
        }
        else
        {
            if (i == 0)
            {
                DrawCube({ worldX, 0.5f, worldZ }, 1.0f, 1.0f, 1.0f, GREEN);
            }
            else
            {
                DrawCube({ worldX, 0.5f, worldZ }, 0.9f, 0.9f, 0.9f, LIME);
            }
        }
    }
}