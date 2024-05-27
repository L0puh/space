#ifndef STATE_H
#define STATE_H

#include "game.h"

struct global {
   GLFWwindow* window;
   window_size w_size;
   Camera* camera;
   User* user;
   boarder cur_boarder;
   struct collision {
      bool AABB_AABB;
      bool circle_AABB;
   };
   collision collision_type;
   float zoom = 1.0f;
   float timestep = 0.5f;
   float gravity = 10.f;
   float stars_amount = 0.1f;
};

extern global global_states;

#endif
