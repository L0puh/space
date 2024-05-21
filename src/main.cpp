#include "game.h" 
#include "state.h"
#include "collision.h"
#include "utils.h"

#include <GLFW/glfw3.h>
#include <vector>

#define DEBUG_MODE
/* #define COLLISION_PROTOTYPE */

global global_states;

int main() {
   const float width = 400, height = 400;
   GLFWwindow *window = init_window(width, height);
   global_states.window = window;
   global_states.w_size = {(int)width, (int)height};

   glEnable(GL_BLEND); // transparent bg for textures (RBGA)
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG_MODE
   utils::init_debug_console();
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(message_callback, 0);
   utils::log("DEBUG MODE");
#endif 

   User   user("../shaders/user.vert", "../shaders/user.frag", "../textures/spaceship.png", PNG);
   Planet planet("../shaders/user.vert", "../shaders/user.frag", "../textures/planet.png", PNG, 5.0f);
   Object star("../shaders/standard.vert", "../shaders/standard.frag", NONE);

#ifdef COLLISION_PROTOTYPE
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype();
#endif

   Camera camera;
   camera.set_position({1.0f, 1.0f, 0.0f});
   user.pos = camera.pos;
   
   std::vector<collider> objs; //FIXME
   objs.push_back({planet.pos, planet.size, planet.radius});

   float last_frame = 0.0f, deltatime;

   global_states.camera = &camera;
   global_states.user = &user;
   global_states.window = window;
   global_states.w_size = get_window_size(window);

#ifndef COLLISION_PROTOTYPE
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      glfwSetKeyCallback(window, Input::key_callback);
      
      camera.update();
      planet.update();
      user.update();
      star.update();

      planet.translate_object(planet.pos);
      planet.scale_object({1.f, 1.f});
      objs[0] = {planet.pos, planet.size, planet.size.x/sqr_2};
            
      camera.get_movement(deltatime, user.size, objs); 
      user.translate_object(camera.pos);
      user.rotate_object(camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.scale_object(user.size);
      star.translate_object(camera.pos);

      glClearBufferfv(GL_COLOR, 0, bg);
      utils::debug_new_frame();
      utils::debug_console();

      //objects.draw(); 
      generate_objs(planet, 1, circle);
      generate_objs(star, 0.14, dot);
      planet.draw(planet.model, camera.view);
      user.draw(user.model, camera.view);
     
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
      global_states.camera = &camera;
      global_states.user = &user;
      global_states.window = window;
      global_states.w_size = get_window_size(window);
   }
#endif 
   shut_down();
   return 0;
}
