#include "game.h" 
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cstdlib>

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
   Planet planet("../shaders/user.vert", "../shaders/user.frag", "../textures/planet.png", PNG, 1.0f);
   Object bg_obj("../shaders/user.vert", "../shaders/user.frag", "../textures/bg.jpg", JPG);
   Object star("../shaders/standard.vert", "../shaders/standard.frag");
   Camera camera;
   
   float last_frame = 0.0f, deltatime;
   while (!glfwWindowShouldClose(window)){
      deltatime = get_deltatime(&last_frame); 
      glfwSetKeyCallback(window, Input::key_callback);
      camera.get_movement(window, deltatime); 
      
      //objects.update();
      camera.update();
      planet.update();
      user.update();
      bg_obj.update();
    
      bg_obj.scale_object(window, glm::vec2(10.0f, 10.0f));
      user.translate_object(window, camera.pos);
      user.rotate_object(window, camera.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      user.scale_object(window, glm::vec2(0.3f, 0.3f));
      planet.scale_object(window, glm::vec2(1.0f, 1.0f));
      
      glClearBufferfv(GL_COLOR, 0, bg);
      utils::debug_new_frame();
      utils::debug_console(window, &user, &camera);

      //objects.draw(); 
      planet.draw(window, planet.model, camera.view);
      user.draw(window, user.model, camera.view);
     
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   shut_down(window);
   return 0;
}
