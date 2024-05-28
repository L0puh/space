#ifndef STATE_H
#define STATE_H

#include "game.h"

struct global {
   GLFWwindow* window;
   window_size w_size;
   Camera* camera;
   User* user;
   struct collision {
      bool AABB_AABB;
      bool circle_AABB;
   };
   boarder cur_boarder;
   collision collision_type;
   float zoom = 10.0f;
   float timestep = 0.5f;
   float gravity = 10.f;
   float stars_amount = 0.8f;
};

extern global global_states;

#endif
