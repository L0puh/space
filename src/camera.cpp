#include "game.h"
#include "collision.h"
#include "glm/ext/matrix_transform.hpp"
#include <vector>

void Camera::update(){
   view = glm::translate(glm::mat4(1.0f), pos);
   
   view = glm::translate(view, {0.5 * 0.2, 0.5*0.2, 0.0f}); //FIXME: user size 
   view = glm::rotate(view, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
   view = glm::translate(view, {-0.5 * 0.2, -0.5*0.2, 0.0f});

   view = glm::inverse(view);
}

bool Camera::check_boarder(glm::vec2 map_size, glm::vec2 user_size, float x, float y){
   bool pos = (x-user_size.x >= (map_size.x-map_offset)/2 || y-user_size.y >= (map_size.y-map_offset)/2);
   bool neg = (-x-user_size.x >= (map_size.x-map_offset)/2 || -y-user_size.y >= (map_size.y-map_offset)/2);
   return pos || neg;
}

bool Camera::check_collisions(std::vector<TEST_obj> objs, glm::vec2 user_size, glm::vec2 pos){
   for (const auto& obj: objs)
      if (AABB_collision(obj, user_size, pos)) return true;
   return false;
}

void Camera::get_movement(GLFWwindow* window, 
      float deltatime, 
      glm::vec2 map_size, 
      glm::vec2 user_size,
      std::vector<TEST_obj> objs) {
   if (Input::is_pressed(window, GLFW_KEY_A)){
      rotation += speed * deltatime;
   } 
   if (Input::is_pressed(window, GLFW_KEY_D)){
       rotation -= speed * deltatime;
   }
   if (Input::is_pressed(window, GLFW_KEY_W)){
      float x=pos.x,y=pos.y;
      x += -sin(rotation) * speed * deltatime;
      y += cos(rotation) * speed * deltatime;
      if (check_boarder(map_size, user_size, x, y) || check_collisions(objs, user_size, {x, y})) {
         rotation-=speed*deltatime;
      } else 
         pos = {x, y, pos.z};
   }
   if (Input::is_pressed(window, GLFW_KEY_S)){
      float x = pos.x, y = pos.y;
      x -= -sin(rotation) * speed * deltatime;
      y -= cos(rotation) * speed * deltatime;
      if (check_boarder(map_size, user_size, x, y) || check_collisions(objs, user_size, {x,y}))
         rotation+=speed*deltatime;
      else
         pos.x = x; pos.y = y;
   }
}

