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
      glfwSetKeyCallback(window, Input::key_callback);
      user.move_object(window); 

      utils::debug_new_frame();
      utils::debug_console(window, &user);
      glClearBufferfv(GL_COLOR, 0, bg);

      user.draw();
      utils::debug_console_render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   shut_down(window);
   return 0;
}
