/*#pragma once
#pragma once

#include "backdrop.hpp"
#include <vector>
#include "../../includes.h"

class Dot;
extern std::vector<Dot*> Dots;

class Dot
{
public:
    int animationalpha_elen = 0;
    Dot(vec2_t p, vec2_t v)
    {
        RelativePosition = p;
        Velocity = v;
    }
  /*  void Draw()
    {

        D3D::RectFilled(vec2_t(RelativePosition.x - 2, RelativePosition.y - 2), vec2_t(4, 4), Color(255, 255, 255 ,255));
        auto t = std::find(Dots.begin(), Dots.end(), this);
        if (t == Dots.end()) return;
        for (auto i = t; i != Dots.end(); i++)
        {
            if ((*i) == this) continue;
            vec2_t Pos = RelativePosition;
            float Dist = Pos.DistTo((*i)->RelativePosition);
            if (Dist < 128)
            {

            }
        }
    }
    vec2_t RelativePosition = vec2_t(0, 0);
    float Rotation = 0;
    float Size = 0;
    vec2_t Velocity;
};

namespace BackDrop
{
    void DrawBackDrop();
}*/