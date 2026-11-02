#include "Debug.h"

#include <cassert>

void Debug::Draw(SDL_Renderer* renderer, TTF_TextEngine* textEngine, TTF_Font* font, int layer)
{
    for (const Line& line : mLines[layer])
    {
        line.Draw(renderer);
    }

    for (const Rectangle& rect : mRectangles[layer])
    {
        rect.Draw(renderer);
    }

    for (const Circle& circle : mCircles[layer])
    {
        circle.Draw(renderer);
    }

    for (const Text& text : mTexts[layer])
    {
        text.Draw(textEngine, font);
    }
}

void Debug::Clear()
{
    for (int i = 0; i < 2; ++i)
    {
        mLines[i].clear();
        mRectangles[i].clear();
        mCircles[i].clear();
        mTexts[i].clear();
    }
}

void Debug::DrawLine(Vector2f a, Vector2f b, const Color& color, bool front)
{
    Line line;
    line.a = a;
    line.b = b;
    line.color = color;

    mLines[front ? 1 : 0].push_back(line);
}

void Debug::DrawRectangle(Vector2f topLeft, Vector2f size, const Color& color, bool filled, Vector2f ratio, bool front)
{
    Rectangle rect;

    rect.position = topLeft - (ratio * size);
    rect.size = size;
    rect.color = color;
    rect.filled = filled;

    mRectangles[front ? 1 : 0].push_back(rect);
}

void Debug::DrawCircle(Vector2f center, float radius, const Color& color, bool filled, bool front)
{
    Circle circle;

    circle.position = center - radius;
    circle.radius = radius;
    circle.color = color;
    circle.filled = filled;

    mCircles[front ? 1 : 0].push_back(circle);
}

void Debug::DrawText(Vector2f position, const std::string& text, const Color& color, Vector2f ratio, bool front)
{
    assert(ratio.x >= 0.f && ratio.x <= 1.f);
    assert(ratio.y >= 0.f && ratio.y <= 1.f);

    if (text.empty())
        return;

    mTexts[front ? 1 : 0].push_back({ position, text, color, ratio.x, ratio.y });
}
