#pragma once
#include "surface.h"
#include "template.h"
#include <list>
#include "Level.h"
#include "CollisionDetection.h"
#include <functional>

class Level;


    enum class Direction {
        LEFT,
        RIGHT
    };

    class Slime
    {
    public:
        using FinishCallback = std::function<void()>;

        Slime(Surface* surface, Level& level, FinishCallback onFinish);
        void Draw(float deltaTime);
        void UpdateSprite(float deltaTime);
        void Jump();
        void KeyDown(int key);
        void KeyUp(int key);
        void CheckCollision(bool& collidingFromTop, bool& collidingFromLeft, bool& collidingFromRight);
        void HandleCollision(LevelBlock block, RectangleSide side, vec4 line, bool& collidingFromTop, bool& collidingFromLeft, bool& collidingFromRight);
        void Move(Direction direction);
        void UpdateIsGrounded();
    private:
        Surface* screen;
        Level& m_level;
        Direction m_SlimeDirection;

        float m_initialSpeedXWhenJumping;
        float m_timeSinceLastFrameUpate = 0.0f;
        float m_stickyEffectTimer = 0.0f;
        bool m_climbingSticky = false;

        void UpdateSlimeSpeedY(float deltaTime);
        void UpdateClimbingSticky(float deltaTime);

        FinishCallback m_onFinish;
    };
