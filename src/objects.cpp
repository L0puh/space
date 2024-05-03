#include "game.h"
#include <glm/ext/matrix_transform.hpp>
#include <string>

Object::Object(std::string src_vertex, std::string src_fragment) : shader(src_vertex, src_fragment){
   vertex.create_VBO(data_square, sizeof(data_square));
   vertex.create_EBO(indices_square, sizeof(indices_square));
   vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
}
Object::~Object(){
   shader.delete_shader();
   texture.delete_texture();
}
void Object::scale_object(GLFWwindow *window, glm::mat4 *model, glm::vec2 scaler){
   *model = glm::scale(*model, glm::vec3(scaler.x, scaler.y, 0));
   
}
void Object::translate_object(GLFWwindow *window, glm::mat4 *model, glm::vec2 pos){
   *model = glm::translate(*model, glm::vec3(pos.x, pos.y, 0.0f));
   
}
void Object::rotate_object(GLFWwindow *window, glm::mat4 *model, float angle, glm::vec3 pos){
   *model = glm::rotate(*model, angle, pos);
}

void Object::draw(GLFWwindow* window, glm::mat4 &model){
   shader.use();
   shader.set_matrix4fv("model", model);
   vertex.draw(GL_TRIANGLES, LEN(indices_square));
}
/****************************************************/
/*                   USER                          */


void User::get_movement(GLFWwindow* window, glm::mat4 *model) {
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
   translate_object(window, model, glm::vec2(moving_direction.right_left, moving_direction.up_down));
}
/****************************************************/
/*                   PLANET                        */

