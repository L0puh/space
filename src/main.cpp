#include "game.h" 
#include "collision.h"
#include "utils.h"

#include <GLFW/glfw3.h>
#include <vector>

#define DEBUG_MODE
#define COLLISION_PROTOTYPE


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
   Object bg_obj("../shaders/user.vert", "../shaders/user.frag", "../textures/bg.jpg", JPG);
   Object star("../shaders/standard.vert", "../shaders/standard.frag", NONE);

#ifdef COLLISION_PROTOTYPE
   collider c1, c2;
   /* Collision_prototype coll_p(Collision_prototype::AABB_AABB, object_type::circle, object_type::square); */
   Collision_prototype coll_p(Collision_prototype::AABB_AABB, square, square);
   coll_p.update_prototype(window, &c1, &c2);
#endif

   Camera camera;
   camera.set_position({1.0f, 1.0f, 0.0f});
   user.pos = camera.pos;
   
   std::vector<collider> objs; //FIXME
   objs.push_back({planet.pos, planet.size, planet.radius});

   glm::vec2 bg_scale = {10.0f, 10.0f};
   float last_frame = 0.0f, deltatime;

#ifndef COLLISION_PROTOTYPE
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      camera.set_window(window);
      glfwSetKeyCallback(window, Input::key_callback);
      
      camera.update();
      planet.update();
      user.update();
      bg_obj.update();

      bg_obj.scale_object(bg_scale);
      planet.translate_object(planet.pos);
      planet.scale_object(user.size);

      objs[0] = {planet.pos, planet.size, planet.size.x/sqr_2};
      
      camera.get_movement(window, deltatime, bg_scale, user.size, objs); 
      user.translate_object(camera.pos);
      user.rotate_object(camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.scale_object(user.size);


      glClearBufferfv(GL_COLOR, 0, bg);
      utils::debug_new_frame();
      utils::debug_console(window, &user, &camera);

      //objects.draw(); 
      bg_obj.draw(window, bg_obj.model, camera.view);
      planet.draw(window, planet.model, camera.view);
      /* star.draw(window, user.model, camera.view); */
      user.draw(window, user.model, camera.view);
     
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
#endif 
   shut_down(window);
   return 0;
}
