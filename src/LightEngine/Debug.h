#pragma once

#include <vector>
#include <string>
#include "Rendering.h"
#include "Color.h"

class GameManager;
struct TTF_Font;
struct TTF_TextEngine;

class Debug
{
    Debug() = delete;

    inline static std::vector<Line> mLines[2];
    inline static std::vector<Rectangle> mRectangles[2];
    inline static std::vector<Circle> mCircles[2];
    inline static std::vector<Text> mTexts[2];

    static void Draw(SDL_Renderer* renderer, TTF_TextEngine* textEngine, TTF_Font* font, int layer);
    static void Clear();

public:
    static void DrawLine(Vector2f a, Vector2f b, const Color& color, bool front = true);
    static void DrawRectangle(Vector2f topLeft, Vector2f size, const Color& color, bool filled, Vector2f ratio = { 0.f, 0.f }, bool front = true);
    static void DrawCircle(Vector2f center, float radius, const Color& color, bool filled, bool front = true);
    static void DrawText(Vector2f topLeft, const std::string& text, const Color& color = Color::White, Vector2f ratio = { 0.f, 0.f }, bool front = true);

    friend class GameManager;
};
