#include "game.h" 
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#define DEBUG_MODE

int main() {

   GLFWwindow *window = init_window(400, 400);

#ifdef DEBUG_MODE
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(message_callback, 0);
   utils::log("DEBUG MODE");
#endif 

   Shader standard = Shader("../shaders/standard.vert", "../shaders/standard.frag");
   Vertex vrx, vrx2;

   direction move;
   std::string label = "model";

   vrx.create_VBO(data_square, sizeof(data_square)); 
   vrx.create_EBO(indices_square, sizeof(indices_square));
   vrx.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
   
   vrx2.create_VBO(data_triangle, sizeof(data_triangle)); 
   vrx2.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

   while (!glfwWindowShouldClose(window)){
      glfwSetKeyCallback(window, Input::key_callback);
      Input::move_object(window, &move); 
      
      glClearBufferfv(GL_COLOR, 0, bg);

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(move.right_left, move.up_down, 0.0f));

      standard.use();
      standard.set_matrix4fv(label, model);
      vrx.draw(GL_TRIANGLES, LEN(indices_square));
      vrx2.draw_buffer(GL_TRIANGLES, 3);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   shut_down(window);
   return 0;
}
