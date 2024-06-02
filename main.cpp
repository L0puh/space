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
   Texture_sheet coord;
   coord = {0, 1};
   User user("../shaders/user.vert", "../shaders/user.frag", &tex_sheet, coord);
   coord = {0, 3};
   Planet planet("../shaders/user.vert", "../shaders/user.frag", &tex_sheet, coord);
   Object star("../shaders/standard.vert", "../shaders/standard.frag", NONE);
   Object dot("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE); 
   Galaxy galaxy(&dot, {300.0f, 4.0f}, 100);
   coord = {0, 2};
   Black_hole hole("../shaders/user.vert", "../shaders/user.frag", &tex_sheet, coord);

#ifdef COLLISION_PROTOTYPE
   global_states.zoom=1.0f;
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype();
#endif

   Camera camera;
#ifdef ORBIT_PROTOTYPE
   camera.view = glm::mat4(1.0f);
   global_states.camera = &camera;
   orbit::run_orbit_prototype();
#endif
   camera.set_init_position();
   user.pos = camera.pos;
   

   float last_frame = 0.0f, deltatime;

   global_states.camera = &camera;
   global_states.user = &user;
   global_states.window = window;
   global_states.w_size = get_window_size(window);

   size_t amount_planets = 10;
   std::vector<collider> objs(amount_planets); //FIXME
   Map map(&objs, amount_planets, &planet, galaxy.center_pos);
   std::vector<glm::vec2> stars(7000);

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

      hole.translate_object(glm::vec2(8.0f, 8.0f) - glm::vec2(camera.pos.x, camera.pos.y)); 
      hole.scale_object({4.0f, 2.0f});

      camera.get_movement(deltatime, user.size, objs); 
      hole.collide(&camera, galaxy.center_pos-map.get_planet(0).size, user.size);
      user.translate_object(camera.inital_pos);
      user.scale_object(user.size);
      user.rotate_object(camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.set_pos(camera.pos);
      
      printf("%d\n", galaxy.is_out(&user));

      glClearBufferfv(GL_COLOR, 0, bg_color);
      utils::debug_new_frame();
      utils::debug_console();

      //objects.draw(); 
      /* galaxy.generate_galaxy_sphere(stars.size(), &stars); */
      /* galaxy.draw_galaxy_sphere(stars); */
      map.draw_planets();

      galaxy.generate_galaxy_procedural();
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
