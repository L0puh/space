/*******************************************

            ORBIT PROTOTYPE

*******************************************/
#ifndef ORBIT_H
#define ORBIT_H

#include "game.h"

struct planet_objects{
   float radius;
   glm::vec2 pos;
   glm::vec2 velocity = {0.0f, 0.0f};
   float mass;
   glm::vec2 size;
   float distance_to_center;
   std::vector<glm::vec2> orbit;
};

void draw_planets(planet_objects planets[], size_t amount, Planet *planet, Object *dot);
void draw_orbit(std::vector<glm::vec2> orbit, Object *dot);
void run_orbit_prototype();


#endif
