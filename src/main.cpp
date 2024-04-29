#include "game.h" 

#define DEBUG_MODE

int main() {

   GLFWwindow *window = init_window(400, 400);
#ifdef DEBUG_MODE
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(message_callback, 0);
   utils::log("DEBUG MODE");
#endif 

   while (!glfwWindowShouldClose(window)){
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT);
      glfwSetKeyCallback(window, key_callback);
      glfwSwapBuffers(window);
   }
   
   shut_down(window);
   return 0;
}
