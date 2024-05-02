#include "game.h"
#include <glm/ext/matrix_transform.hpp>

/* Object::Object(){ */
/* } */


/****************************************************/
/*                   USER                          */

User::User(std::string src_vertex, std::string src_fragment): shader(src_vertex, src_fragment){
   vertex.create_VBO(data_square, sizeof(data_square));
   vertex.create_EBO(indices_square, sizeof(indices_square));
   vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
}
User::~User(){
   shader.delete_shader();
   //delete stuff...
}
void User::draw(){
   glm::mat4 model = glm::mat4(1.0f);
   model = glm::translate(model, glm::vec3(moving_direction.right_left, moving_direction.up_down, 0.0f));

   shader.use();
   shader.set_matrix4fv("model", model);
   vertex.draw(GL_TRIANGLES, LEN(indices_square));
}
void User::move_object(GLFWwindow* window) {
   if (Input::is_pressed(window, GLFW_KEY_A)){
      if (moving_direction.right_left - speed <= -1) return;
      moving_direction.right_left -= speed;
   } 
   if (Input::is_pressed(window, GLFW_KEY_D)){
      if (moving_direction.right_left + speed >= 1) return;
      moving_direction.right_left += speed;
   }
   if (Input::is_pressed(window, GLFW_KEY_W)){
      if (moving_direction.up_down + speed >= 1) return;
      moving_direction.up_down += speed;
   }
   if (Input::is_pressed(window, GLFW_KEY_S)){
      if (moving_direction.up_down - speed <= -1) return;
      moving_direction.up_down -= speed;
   }
}
/****************************************************/
/*                   PLANET                        */

Planet::Planet(std::string src_vertex, std::string src_fragment) : shader(src_vertex, src_fragment){
   vertex.create_VBO(data_square, sizeof(data_square));
   vertex.create_EBO(indices_square, sizeof(indices_square));

}
