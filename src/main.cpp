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
   Vertex vrx;
  
   float data[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
   };

   vrx.create_VBO(data, sizeof(data)); 
   vrx.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);


   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, bg);
      
      standard.use();
      vrx.draw_buffer(GL_TRIANGLES, 3);
      glfwSetKeyCallback(window, Input::key_callback);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   shut_down(window);
   return 0;
}
