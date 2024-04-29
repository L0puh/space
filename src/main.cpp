#include "game.h" 

#define DEBUG_MODE

int main() {

   GLFWwindow *window = init_window(400, 400);

#ifdef DEBUG_MODE
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(message_callback, 0);
   utils::log("DEBUG MODE");
#endif 

   Shader standard("../shaders/standard.vert", "../shaders/standard.frag");

   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, bg);


      glfwSetKeyCallback(window, key_callback);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   shut_down(window);
   return 0;
}
