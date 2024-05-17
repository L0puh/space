#include "game.h" 
#include "collision.h"
#include "glm/geometric.hpp"
#include "utils.h"

#include <GLFW/glfw3.h>
#include <vector>

#define DEBUG_MODE
/* #define COLLISION_PROTOTYPE */

int main() {
   const float width = 400, height = 400;
   GLFWwindow *window = init_window(width, height);
   glEnable(GL_BLEND); // transparent bg for textures (RBGA)
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG_MODE
   utils::init_debug_console(window);
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(message_callback, 0);
   utils::log("DEBUG MODE");
#endif 

   User   user("../shaders/user.vert", "../shaders/user.frag", "../textures/spaceship.png", PNG);
   Planet planet("../shaders/user.vert", "../shaders/user.frag", "../textures/planet.png", PNG, 5.0f);
   Object star("../shaders/standard.vert", "../shaders/standard.frag", NONE);

#ifdef COLLISION_PROTOTYPE
   collider c1, c2;
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype(window, &c1, &c2);
#endif

  //FIXME: create global state 
   Camera camera;
   camera.set_position({1.0f, 1.0f, 0.0f});
   user.pos = camera.pos;
   
   std::vector<collider> objs; //FIXME
   objs.push_back({planet.pos, planet.size, planet.radius});

   glm::vec2 bg_scale = {10.0f, 10.0f};
   float last_frame = 0.0f, deltatime;
   float map_offset = 3.0f, amount = 0.1;
   struct boarder {
      float max_x = 3.0f;
      float min_x = -3.0f;
      float max_y = 3.0f;
      float min_y = -3.0f;
   };
   boarder cur_boarder;
   float prev_x = 3.0f, prev_y = 3.0f;
#ifndef COLLISION_PROTOTYPE
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      camera.set_window(window);
      glfwSetKeyCallback(window, Input::key_callback);
      
      camera.update();
      planet.update();
      user.update();
      star.update();

      planet.translate_object(planet.pos);
      planet.scale_object({1.f, 1.f});
      objs[0] = {planet.pos, planet.size, planet.size.x/sqr_2};
            
      camera.get_movement(window, deltatime, bg_scale, user.size, objs); 
      user.translate_object(camera.pos);
      user.rotate_object(camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.scale_object(user.size);
      star.translate_object(camera.pos);


      glClearBufferfv(GL_COLOR, 0, bg);
      utils::debug_new_frame();
      utils::debug_console(window, &user, &camera);

      if (camera.pos.y+1.0f >= cur_boarder.max_y || camera.pos.x+1.0f >= cur_boarder.max_x) {
         prev_x = cur_boarder.max_x;
         prev_y = cur_boarder.max_y;
         cur_boarder.max_y = camera.pos.y+map_offset;
         cur_boarder.min_x = camera.pos.x-map_offset;
         cur_boarder.max_x = camera.pos.x+map_offset;
         cur_boarder.min_y = camera.pos.y-map_offset;
      }
      if (camera.pos.y-1.0f <= cur_boarder.min_y || camera.pos.x-1.0f <= cur_boarder.min_x) {
         cur_boarder.max_y = camera.pos.y-map_offset;
         cur_boarder.min_x = camera.pos.x+map_offset;
         cur_boarder.max_x = camera.pos.x-map_offset;
         cur_boarder.min_y = camera.pos.y+map_offset;
      }
      srand(1000);
      if (Input::is_relesed(window, GLFW_KEY_SPACE)){
         for ( float i=cur_boarder.min_x; i <= cur_boarder.max_x; i+=amount){
            for (float j=cur_boarder.min_y; j <= cur_boarder.max_y; j+=amount){
               bool is_star = rand() % 256 < 32;
               if (is_star) {
                  star.update();
                  star.translate_object(glm::vec2(i, j));
                  star.draw(window, star.model, camera.view, white);
               }
            }
         }
      }
      //objects.draw(); 
      planet.draw(window, planet.model, camera.view);
      user.draw(window, user.model, camera.view);
     
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
#endif 
   shut_down(window);
   return 0;
}
