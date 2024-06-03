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
  
   Texture tex_sheet("../textures/texture_sheet.png", PNG);
   tex_sheet.load_texture();

   User user("../shaders/user.vert", "../shaders/user.frag", &tex_sheet);
   Planet planet("../shaders/user.vert", "../shaders/user.frag", &tex_sheet); 
   Object star("../shaders/standard.vert", "../shaders/standard.frag", NONE);
   Object dot("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE); 
   Black_hole hole("../shaders/user.vert", "../shaders/user.frag", &tex_sheet);
   Camera camera;

#ifdef COLLISION_PROTOTYPE
   global_states.zoom=1.0f;
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype();
#endif

#ifdef ORBIT_PROTOTYPE
   camera.view = glm::mat4(1.0f);
   global_states.camera = &camera;
   orbit::run_orbit_prototype(&planet, &dot);
#endif
   camera.set_init_position();
   user.pos = camera.pos;

   float last_frame = 0.0f, deltatime;

   global_states.camera = &camera;
   global_states.user = &user;
   global_states.window = window;
   global_states.w_size = get_window_size(window);

   size_t amount_planets = 10, amount_black_holes=40;
   std::vector<collider> objs(amount_planets); //FIXME
   std::vector<glm::vec2> stars(7000);
   std::vector<black_hole_object> black_holes(amount_black_holes);
   
   Galaxy galaxy(amount_planets, {300.0f, 4.0f}, 1000, &star, &planet);
   galaxy.init_map(&objs);

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
      galaxy.update(&objs);
      galaxy.generate_black_holes(amount_black_holes, &black_holes);


      camera.get_movement(deltatime, user.size, objs); 
      for (int i=0; i < amount_black_holes; i++){
         hole.collide(&camera, black_holes[i].to, user.size);  
      }

      hole.update(); 
      hole.translate_object(glm::vec2(8.0f, 8.0f) - glm::vec2(camera.pos.x, camera.pos.y)); 
      hole.scale_object({4.0f, 2.0f});
      hole.collide(&camera, galaxy.center_pos-galaxy.get_planet(0).size, user.size);

      user.translate_object(camera.inital_pos);
      user.scale_object(user.size);
      user.rotate_object(camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.set_pos(camera.pos);

      glClearBufferfv(GL_COLOR, 0, bg_color);
      utils::debug_new_frame();
      utils::debug_console();

      //objects.draw(); 
      
      galaxy.generate_galaxy_sphere(stars.size(), &stars);
      galaxy.draw_galaxy_sphere(stars);
      galaxy.draw_planets();
      galaxy.draw_black_holes(amount_black_holes, black_holes, &hole);

      hole.update(); 
      hole.translate_object(glm::vec2(8.0f, 8.0f) - glm::vec2(camera.pos.x, camera.pos.y)); 
      hole.scale_object({4.0f, 2.0f});
      hole.draw(hole.model, camera.view);
      user.draw(user.model, camera.view);

      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
      global_states.camera = &camera;
      global_states.user = &user;
      global_states.window = window;
      global_states.w_size = get_window_size(window);
      global_states.deltatime = deltatime;
   }
#endif 
#endif 
   shut_down();
   return 0;
}
