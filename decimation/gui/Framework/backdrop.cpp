/*#include "backdrop.hpp"
#include <vector>
#include "../../includes.h"

std::vector<Dot*> Dots;

void BackDrop::DrawBackDrop()
{

    static int width = g_cl.m_width, height = g_cl.m_height;


    int s = rand() % 50;

    if (s == 0)
        Dots.push_back(new Dot(vec2_t(rand() % (int)width, -16), vec2_t((rand() % 7) - 3, rand() % 3 + 1)));
    else if (s == 1)
        Dots.push_back(new Dot(vec2_t(rand() % (int)width, (int)height + 16), vec2_t((rand() % 7) - 3, -1 * (rand() % 3 + 1))));
    else if (s == 2)
        Dots.push_back(new Dot(vec2_t(-16, rand() % (int)height), vec2_t(rand() % 3 + 1, (rand() % 7) - 3)));
    else if (s == 3)
        Dots.push_back(new Dot(vec2_t((int)width + 16, rand() % (int)height), vec2_t(-1 * (rand() % 3 + 1), (rand() % 7) - 3)));

    for (auto i = Dots.begin(); i < Dots.end();)
    {
        if ((*i)->RelativePosition.y < -20 || (*i)->RelativePosition.y > height + 20 || (*i)->RelativePosition.x < -20 || (*i)->RelativePosition.x > width + 20)
        {
            delete (*i);
            i = Dots.erase(i);
        }
        else
        {
            (*i)->RelativePosition = (*i)->RelativePosition + (*i)->Velocity * (1.f);
            i++;
        }
    }
    for (auto i = Dots.begin(); i < Dots.end(); i++);
   /*     (*i)->Draw();*/