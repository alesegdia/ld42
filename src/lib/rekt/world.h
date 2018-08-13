#pragma once

#include <vector>
#include <cstdint>
#include <algorithm>

namespace rekt
{

typedef int rekt_t;

struct V2
{
    rekt_t x;
    rekt_t y;
};

template <typename T>
struct Rect
{
    T x, y, w, h;
    T left()
    {
        return x;
    }
    T right()
    {
        return x + w;
    }
    T top()
    {
        return y;
    }
    T bottom()
    {
        return y + h;
    }

};

enum class CollisionType : uint8_t
{
    Slide,
    Sensor
};

struct Entity;

struct SingleCollisionInfo
{
    Entity* e;
    V2 normal;
};

struct MovementResponse
{
    std::vector<SingleCollisionInfo> collisions;
};

class MovementAnalyzer
{
public:
    void reset(V2 old_pos, V2 new_pos)
    {
        delta.x = new_pos.x - old_pos.x;
        delta.y = new_pos.y - old_pos.y;
    }

    bool hasHorizontalMovement()
    {
        return delta.x != 0;
    }

    bool hasVerticalMovement()
    {
        return delta.y != 0;
    }

    bool isMovingLeft()
    {
        return delta.x > 0;
    }

    bool isMovingRight()
    {
        return delta.x < 0;
    }

    bool isMovingUp()
    {
        return delta.y > 0;
    }

    bool isMovingDown()
    {
        return delta.y < 0;
    }

private:
    V2 delta = {0, 0};

};


struct Entity
{
    Rect<rekt_t> rect;
    uint8_t category;
    MovementResponse __cachedMovementResponse;
    MovementAnalyzer __cachedMovementAnalyzer;

    rekt_t x()
    {
        return rect.x;
    }
    rekt_t y()
    {
        return rect.y;
    }
};


class World
{
public:

    typedef void (*CollisionResponse)(Entity& me, Entity& other, SingleCollisionInfo cdata);

    MovementResponse* move(Entity* requester, rekt_t x, rekt_t y, CollisionResponse cr = nullptr )
    {
        MovementResponse* response = &(requester->__cachedMovementResponse);
        MovementAnalyzer* analyzer = &(requester->__cachedMovementAnalyzer);
        analyzer->reset( { requester->x(), requester->y() }, {x, y} );
        std::vector<Entity*> near_entities = broadphase(requester);
        for( auto candidate_entity : near_entities )
        {
            handlePossibleCollision(requester, candidate_entity, {x, y}, response, analyzer);
        }
        return response;
    }

    void handlePossibleCollision(Entity* requester, Entity* candidate, V2 new_pos, MovementResponse* response, MovementAnalyzer* analyzer )
    {
        // We assume requester IS moving and candidate IS NOT
        // We assume that Y coordinate increases by going down
        //
        // If one rectangle is on left side of other
        //      if (a.left > b.right || b.left > a.right) return false;
        //
        // If one rectangle is above other
        //      if (a.top < b.bottom || b.top < a.bottom) return false;
        //
        auto& a = requester->rect;
        auto& b = candidate->rect;

        // a.top - b.bottom > 0
        rekt_t dydist1 = a.top() - b.bottom();

        // a.top - a.bottom > 0
        rekt_t dydist2 = b.top() - a.bottom();

        // a.left - b.right > 0
        rekt_t dxdist1 = a.left() - b.right();

        // a.left - a.right > 0
        rekt_t dxdist2 = b.left() - a.right();

        if( analyzer->hasHorizontalMovement() )
        {

            if( analyzer->isMovingLeft() )
            {

            }
            else if( analyzer -> isMovingRight() )
            {

            }
        }
        else if( analyzer->hasVerticalMovement() )
        {

            if( analyzer->isMovingUp() )
            {

            }
            else if( analyzer->isMovingDown() )
            {

            }
        }
    }

    std::vector<Entity*> broadphase(Entity* e)
    {
        std::vector<Entity*> ents = m_entities;
        ents.erase(std::remove(ents.begin(), ents.end(), e), ents.end());
        return ents;
    }

    Entity* newEntity(rekt_t x, rekt_t y, rekt_t w, rekt_t h)
    {
        return nullptr;
    }

private:

    std::vector<Entity*> m_entities;

};

}
