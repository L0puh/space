#include "game.h" 
#include "utils.h"
#include <GLFW/glfw3.h>

#define DEBUG_MODE

int main() {

   GLFWwindow *window = init_window(400, 400);

#ifdef DEBUG_MODE
   utils::init_debug_console(window);
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(message_callback, 0);
   utils::log("DEBUG MODE");
#endif 

   User user("../shaders/user.vert", "../shaders/user.frag");

   while (!glfwWindowShouldClose(window)){
      glm::mat4 model = glm::mat4(1.0f);

      glfwSetKeyCallback(window, Input::key_callback);

      user.get_movement(window, &model); 
      user.rotate_object(window, &model, glfwGetTime(), glm::vec3(0.0, 0.0, 1.0f));
      user.scale_object(window, &model, glm::vec2(0.2, 0.2f));

      utils::debug_new_frame();
      utils::debug_console(window, &user);
      glClearBufferfv(GL_COLOR, 0, bg);

      user.draw(window, model);
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   shut_down(window);
   return 0;
}
