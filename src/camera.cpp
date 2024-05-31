#include "game.h"
#include "collision.h"
#include "glm/ext/matrix_transform.hpp"
#include <vector>


void Camera::update(){
   set_init_position();

   view = glm::translate(view, {0.5 * 0.2, 0.5*0.2, 0.0f}); //FIXME: user size 
   view = glm::rotate(view, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
   view = glm::translate(view, {-0.5 * 0.2, -0.5*0.2, 0.0f});

   view = glm::inverse(view);
}

bool Camera::check_collisions(std::vector<collider> objs, glm::vec2 user_size, glm::vec2 pos){
   for (const auto& obj: objs){
      if (!AABB_collision({obj.pos, obj.size, obj.radius}, {pos, user_size, 2.0f})) 
         return true;
   }
   return false;
}

void Camera::get_movement( float deltatime, glm::vec2 user_size, std::vector<collider> objs) {
   if (Input::is_pressed(GLFW_KEY_A)){
      rotation += rotation_speed * deltatime;
   } 
   if (Input::is_pressed(GLFW_KEY_D)){
       rotation -= rotation_speed * deltatime;
   }
   if (Input::is_pressed(GLFW_KEY_W)){
      float x=pos.x,y=pos.y;
      x += -sin(rotation) * speed * deltatime;
      y += cos(rotation) * speed * deltatime;
      if (check_collisions(objs, user_size, {x, y})) {
         rotation-=rotation_speed*deltatime;
      } else 
         pos = {x, y, pos.z};
   }
   if (Input::is_pressed(GLFW_KEY_S)){
      float x = pos.x, y = pos.y;
      x -= -sin(rotation) * speed * deltatime;
      y -= cos(rotation) * speed * deltatime;
      if (check_collisions(objs, user_size, {x,y}))
         rotation+=rotation_speed*deltatime;
      else
         pos.x = x; pos.y = y;
   }
}

