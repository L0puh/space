#include "game.h" 
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <vector>

#define DEBUG_MODE

float get_deltatime(float *last_time){
   float time, deltatime;
   time = glfwGetTime();
   deltatime = time - *last_time;
   *last_time= time;
   return deltatime;
}

int main() {
   const float width = 400, height = 400;
   GLFWwindow *window = init_window(width, height);
  // transparent bg of textures
   glEnable(GL_BLEND);
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
   Object star("../shaders/standard.vert", "../shaders/standard.frag");
   Camera camera;
   camera.set_position({3.0f, 3.0f, 0.0f});
   user.pos = camera.pos;

   std::vector<TEST_obj> objs;
   objs.push_back({planet.pos, planet.size});

   glm::vec2 bg_scale = {10.0f, 10.0f};
   float last_frame = 0.0f, deltatime;
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      glfwSetKeyCallback(window, Input::key_callback);
      camera.get_movement(window, deltatime, bg_scale, glm::vec2(0.3, 0.3), objs); 
      camera.set_window(window);
      
      //objects.update();
      camera.update();
      planet.update();
      user.update();
      bg_obj.update();
      bg_obj.scale_object(window, bg_scale);
      user.translate_object(window, camera.pos);
      user.rotate_object(window, camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.scale_object(window, glm::vec2(0.2f, 0.2f));

      planet.translate_object(window, glm::vec2(2.0f, 2.0f));
      planet.scale_object(window, glm::vec2(2.0f, 2.0f));
      
      objs[0] = {planet.pos, planet.size};

      glClearBufferfv(GL_COLOR, 0, bg);
      utils::debug_new_frame();
      utils::debug_console(window, &user, &camera);

      //objects.draw(); 
      bg_obj.draw(window, bg_obj.model, camera.view);
      planet.draw(window, planet.model, camera.view);
      star.draw(window, user.model, camera.view);
      user.draw(window, user.model, camera.view);
     
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   shut_down(window);
   return 0;
}
