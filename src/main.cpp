#include "game.h" 
#include "state.h"
#include "collision.h"
#include "utils.h"
#include "orbit.h"

#include <GLFW/glfw3.h>
#include <vector>

#define DEBUG_MODE
/* #define COLLISION_PROTOTYPE */
/* #define ORBIT_PROTOTYPE */

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
   Object dot("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE); 

#ifdef COLLISION_PROTOTYPE
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype();
#endif

#ifdef ORBIT_PROTOTYPE
   run_orbit_prototype();
#endif
   Camera camera;
   camera.set_position({1.0f, 1.0f, 0.0f});
   user.pos = camera.pos;
   

   float last_frame = 0.0f, deltatime;

   global_states.camera = &camera;
   global_states.user = &user;
   global_states.window = window;
   global_states.w_size = get_window_size(window);

   int amount_planets = 4;
   planet_objects planets[amount_planets];

   std::vector<collider> objs(amount_planets); //FIXME

   planets[0] = {150.f, {0.0f, 0.0f}, {0.0f, 0.0f},    155.2f, {12.2, 12.2}, 2.0f};
   planets[1] = {10.0f, {3.7f, 6.7f}, {12.0f, 12.0f},  33.0f,  {4.1, 4.1},   2.0f};
   planets[2] = {20.0f, {4.8f, 5.8f}, {-45.0f, -45.0f},22.2f,  {3.2, 3.2},   2.0f};
   planets[3] = {29.0f, {6.9f, 6.9f}, {67.0f, 67.0f},  22.2f,  {4.2, 4.2},   2.0f};

   for (int i = 0; i < amount_planets; i++){
      objs[i] = {planets[i].pos, planets[i].size, planets[i].radius};
   }
#ifndef COLLISION_PROTOTYPE 
#ifndef ORBIT_PROTOTYPE
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
      
      for (int i = 1; i < amount_planets; i++)
         update_plantes(&planets[i], planets, amount_planets);

      for (int i = 0; i < amount_planets; i++)
         objs[i] = {planets[i].pos, planets[i].size, planets[i].radius};

      glClearBufferfv(GL_COLOR, 0, bg_color);
      utils::debug_new_frame();
      utils::debug_console();

      //objects.draw(); 
      Map::generate_objs(star, global_states.stars_amount, object_type::dot);
      user.draw(user.model, camera.view);
      draw_planets(planets, amount_planets, &planet, &dot);
     
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
      global_states.camera = &camera;
      global_states.user = &user;
      global_states.window = window;
      global_states.w_size = get_window_size(window);
   }
#endif 
#endif 
   shut_down();
   return 0;
}
