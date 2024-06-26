#include "3D.h"
#include "game.h" 
#include "state.h"
#include "orbit.h"
#include "utils.h"

#include <GLFW/glfw3.h>
#include <vector>

#define DEBUG_MODE
/* #define COLLISION_PROTOTYPE */
/* #define ORBIT_PROTOTYPE */
#define PROTOTYPE_3D

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
   Object star("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE);
   Black_hole hole("../shaders/user.vert", "../shaders/user.frag", &tex_sheet);
   Camera camera;
   global_states.user = &user;
#ifdef PROTOTYPE_3D 
   run_3d_prototype();
#endif 
#ifdef COLLISION_PROTOTYPE
   global_states.zoom=1.0f;
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype();
#endif

#ifdef ORBIT_PROTOTYPE
   camera.view = glm::mat4(1.0f);
   global_states.camera = &camera;
   orbit::run_orbit_prototype(&planet, &star);
#endif
   float GALAXIES_AMOUNT = 2;
   std::vector<int> seeds(GALAXIES_AMOUNT);
   std::vector<galaxy_object> galaxies(GALAXIES_AMOUNT);
   orbit::generate_galaxies(galaxies.size(), &seeds, &galaxies);
   Galaxy galaxy(&star, &planet);
   std::vector<glm::vec2> orbits;

   camera.set_init_position(galaxies.at(0).center_pos - galaxies.at(0).scale/10);
   user.set_pos(camera.initial_pos);

   float last_frame = 0.0f, deltatime;

   global_states.camera = &camera;
   global_states.user = &user;
   global_states.window = window;
   global_states.w_size = get_window_size(window);

    
#ifndef COLLISION_PROTOTYPE 
#ifndef ORBIT_PROTOTYPE
#ifndef PROTOTYPE_3D 
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      glfwSetKeyCallback(window, Input::key_callback);
      glfwSetScrollCallback(window, Input::scroll_callback);
     
      camera.update();
      user.update();
      camera.get_movement(deltatime, user.size, galaxies[0].objects);
      orbit::update_galaxies(galaxies.size(), &galaxies, &galaxy, &orbits);
      
      user.translate_object(camera.initial_pos);
      user.scale_object(user.size);
      user.rotate_object(camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.set_pos(camera.pos);

      glClearBufferfv(GL_COLOR, 0, bg_color);
      utils::debug_new_frame();
      utils::debug_console();

      //objects.draw(); 
      orbit::draw_galaxies(galaxies.size(), &galaxies, &galaxy, &hole, &orbits);
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
#endif 
   shut_down();
   return 0;
}
