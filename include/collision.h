/*******************************************

          COLLISION PROTOTYPE
playground with different types of collisions

*******************************************/

#ifndef COLLISION_H
#define COLLISION_H

#include "game.h"

struct collider{
      glm::vec2 pos;
      glm::vec2 size;
      float radius;
};

bool AABB_collision(collider obj, collider obj2);
bool circle_collision(collider obj, collider obj2);

class Collision_prototype {
   public:
      enum collision_type {
         AABB_circle=0,
         AABB_AABB,
      };
      enum object_type {
         circle,
         square
      };
   private:
      Object obj1, obj2;
      collision_type type;
      object_type obj1_t, obj2_t;
   public:
      Collision_prototype(collision_type t, object_type x, object_type y):
         type(t), obj1_t(x), obj2_t(y), 
         obj1("../shaders/standard.vert", "../shaders/standard.frag", LINES),
         obj2("../shaders/standard.vert", "../shaders/standard.frag", LINES) {}
   public:
      bool run_prototype(collider, collider);
      void update_prototype(GLFWwindow* window, collider *c1, collider *c2);
};

#endif 
