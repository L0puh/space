#ifndef STATE_H
#define STATE_H

#include "game.h"

class Camera_3D;

struct global {
   GLFWwindow* window;
   window_size w_size;
   Camera* camera;
   Camera_3D* camera_3d;
   User* user;
   struct collision {
      bool AABB_AABB;
      bool circle_AABB;
   };
   boarder cur_boarder;
   collision collision_type;
   float zoom = 10.0f;
   float timestep = 0.5f;
   float gravity = 7.f;
   float stars_amount = 1.0f;
   float deltatime = 0.0f;
   int last_galaxy = 0;
};

extern global global_states;

#endif
