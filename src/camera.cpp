#include "game.h"
#include "collision.h"
#include "state.h"
#include "3D.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
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
      if (!AABB_collision({obj.pos, obj.size, obj.radius}, {pos+glm::vec2(initial_pos), user_size, 2.0f})) 
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

Camera_3D::Camera_3D(){
   double x, y;
   glfwGetCursorPos(global_states.window, &x, &y);
   last_mouse_pos = {x, y};
}
Camera_3D::~Camera_3D(){
}
glm::mat4 Camera_3D::get_projection(){
   float aspect = (float)global_states.w_size.width/global_states.w_size.height;
   return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
}

void Camera_3D::get_movement(float deltatime){
   if (Input::is_pressed(GLFW_KEY_W)){
      pos += speed * front * deltatime;
   } 
   if (Input::is_pressed(GLFW_KEY_S)){
      pos -= speed * front * deltatime;
   }
   if (Input::is_pressed(GLFW_KEY_A)){
      glm::vec3 d = glm::cross(front, up);
      pos -= glm::normalize(d) * speed * deltatime;
   }
   if (Input::is_pressed(GLFW_KEY_D)){
      glm::vec3 d = glm::cross(front, up);
      pos += glm::normalize(d) * speed * deltatime;
   }
}
void Camera_3D::get_rotation(double xpos, double ypos){
   float xoffset = (xpos-last_mouse_pos.x) * sensitivity;
   float yoffset = (last_mouse_pos.y - ypos) * sensitivity;
   last_mouse_pos = {xpos, ypos};
   
   yaw   += xoffset; 
   pitch += yoffset;
   
   glm::vec3 dir;
   dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   dir.y = sin(glm::radians(pitch));
   dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   front = glm::normalize(dir);
}
glm::mat4 Camera_3D::get_view(){
   glm::mat4 view;
   view = glm::lookAt(pos, pos+front, up);
   return view;
}

