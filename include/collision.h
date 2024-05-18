/*******************************************

          COLLISION PROTOTYPE
playground with different types of collisions

*******************************************/

#ifndef COLLISION_H
#define COLLISION_H
#define sqr_2 1.41421356237f

#include "game.h"

extern global global_states;

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
   private:
      Object obj1, obj2;
      collision_type type;
      object_type obj1_t, obj2_t;
   public:
      Collision_prototype(collision_type t, object_type x, object_type y);
   public:
      bool run_prototype(collider, collider);
      void update_prototype(collider *c1, collider *c2);
};

#endif 
