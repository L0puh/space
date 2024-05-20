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
};

extern global global_states;

#endif
