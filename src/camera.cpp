#include "game.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"

void Camera::update(){
   view = glm::translate(glm::mat4(1.0f), pos);
   rotation_mat = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::get_movement(GLFWwindow* window, float deltatime) {
   //TODO: check boarders
   if (Input::is_pressed(window, GLFW_KEY_A)){
         rotation += speed * deltatime;
   } 
   if (Input::is_pressed(window, GLFW_KEY_D)){
         rotation -= speed * deltatime;
   }
   if (Input::is_pressed(window, GLFW_KEY_W)){
         pos.y += speed * deltatime;
   }
   if (Input::is_pressed(window, GLFW_KEY_S)){
         pos.y -= speed * deltatime;
   }
}

