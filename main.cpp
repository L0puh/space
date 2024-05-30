#include "game.h" 
#include "state.h"
#include "orbit.h"
#include "collision.h"
#include "utils.h"

#include <GLFW/glfw3.h>
#include <ctime>
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
  
   // TEXTURE SHEET (FIXME: create an API for sheet maping)
   Texture tex_sheet("../textures/texture_sheet.png", PNG);
   tex_sheet.load_texture();
   int x_t = 0, y_t = 1;
   float sh_h = 1200, sh_w = 1200;
   float sp_w = 300, sp_h = 300;
   Data data = data_square;
   data = {
      .tex_tr{(x_t*sp_w)/sh_w, (y_t*sp_h)/sh_h},
      .tex_br{((x_t+1)*sp_w)/sh_w, (y_t*sp_h)/sh_h},
      .tex_bl{((x_t+1)*sp_w)/sh_w, ((y_t+1)*sp_h)/sh_h},
      .tex_tl{(x_t*sp_w)/sh_w, ((y_t+1)*sp_h)/sh_h},
   };

   User   user("../shaders/user.vert", "../shaders/user.frag", "../textures/texture_sheet.png", PNG, data);
   Planet planet("../shaders/user.vert", "../shaders/user.frag", "../textures/planet.png", PNG, 5.0f);
   Object star("../shaders/standard.vert", "../shaders/standard.frag", NONE);
   Object dot("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE); 
   Black_hole hole;

#ifdef COLLISION_PROTOTYPE
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype();
#endif

   Camera camera;
#ifdef ORBIT_PROTOTYPE
   camera.view = glm::mat4(1.0f);
   global_states.camera = &camera;
   orbit::run_orbit_prototype();
#endif
   camera.set_position({1.0f, 1.0f, 0.0f});
   user.pos = camera.pos;
   

   float last_frame = 0.0f, deltatime;

   global_states.camera = &camera;
   global_states.user = &user;
   global_states.window = window;
   global_states.w_size = get_window_size(window);


   size_t amount_planets = 10;
   std::vector<collider> objs(amount_planets); //FIXME
   Map map(&objs, amount_planets);
   std::vector<glm::vec2> stars(6000);

#ifndef COLLISION_PROTOTYPE 
#ifndef ORBIT_PROTOTYPE
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      glfwSetKeyCallback(window, Input::key_callback);
      glfwSetScrollCallback(window, Input::scroll_callback);
      
      camera.update();
      planet.update();
      user.update();
      hole.update();
      star.update();
      map.update(&objs);

      hole.translate_object({8.0f, 8.0f}); 
      hole.scale_object({4.0f, 2.0f});

      planet.translate_object(planet.pos);
      planet.scale_object({1.f, 1.f});
      objs[0] = {planet.pos, planet.size, planet.size.x/sqr_2};

      camera.get_movement(deltatime, user.size, objs); 
      hole.collide(&camera, {1.0f, 2.0f}, user.size);
      user.translate_object(camera.pos);
      user.scale_object(user.size);
      user.rotate_object(camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      

      glClearBufferfv(GL_COLOR, 0, bg_color);
      utils::debug_new_frame();
      utils::debug_console();

      //objects.draw(); 
      map.draw_stars();
      map.generate_galaxy(1000, stars.size(), &stars);
      map.draw_galaxy(stars);
      map.draw_planets();
      hole.draw(hole.model, camera.view);
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
#endif 
   shut_down();
   return 0;
}
