#include "game.h" 
#include "utils.h"
#include <GLFW/glfw3.h>

#define DEBUG_MODE

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

   User user("../shaders/user.vert", "../shaders/user.frag", "../textures/spaceship.png", PNG);
   Object bg_obj("../shaders/user.vert", "../shaders/user.frag", "../textures/bg.jpg", JPG);

   while (!glfwWindowShouldClose(window)){
      glm::mat4 model, model_bg;
      model = model_bg =  glm::mat4(1.0f);
      glfwSetKeyCallback(window, Input::key_callback);
      
      bg_obj.scale_object(window, &model_bg, glm::vec2(10.0f, 10.0f));
      
      user.get_movement(window, &model); 
      /* user.rotate_object(window, &model, glfwGetTime(), glm::vec3(0.0, 0.0, 1.0f)); */
      user.scale_object(window, &model, glm::vec2(0.3f, 0.3f));

      utils::debug_new_frame();
      utils::debug_console(window, &user);
   
      glClearBufferfv(GL_COLOR, 0, bg);
      bg_obj.draw(window, model_bg);
      user.draw(window, model);
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   shut_down(window);
   return 0;
}
