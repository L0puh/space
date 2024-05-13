#include "collision.h"
#include "game.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#define sqr_2 1.41421356237f

Collision_prototype::Collision_prototype(collision_type t, object_type x, object_type y):
   type(t), obj1_t(x), obj2_t(y), obj1(x), obj2(y)
{
   if (obj1_t == circle){
      obj1.add_shaders("../shaders/circle.vert", "../shaders/circle.frag");
      obj1.add_texture("../textures/circle.png", PNG);
   } else {
      obj1.add_shaders("../shaders/standard.vert", "../shaders/standard.frag");
      obj1.add_texture("", LINES);
   }
   obj2.add_shaders("../shaders/standard.vert", "../shaders/standard.frag");
   obj2.add_texture("", LINES);
}

bool AABB_collision(collider obj, collider obj2){
   bool x = obj2.pos.x < obj.pos.x + obj.size.x && obj2.pos.x + obj2.size.x > obj.pos.x; 
   bool y = obj2.pos.y < obj.pos.y + obj.size.y && obj2.pos.y + obj2.size.y > obj.pos.y; 
   return x && y;
}

bool circle_collision(collider obj, collider obj2){
   //obj - circle, obj2 - AABB
   glm::vec2 center(obj.pos + obj.radius);
   glm::vec2 AABB_half_extents(obj2.size.x/2.0f, obj2.size.y/2.0f);
   glm::vec2 AABB_center(obj2.pos.x + AABB_half_extents.x,
                         obj2.pos.y + AABB_half_extents.y);
   glm::vec2 difference = center-AABB_center;
   glm::vec2 clamped=glm::clamp(difference, -AABB_half_extents, AABB_half_extents);
   glm::vec2 closest = AABB_center + clamped;
   difference = closest - center;
   return glm::length(difference) < obj.radius;
}

void Collision_prototype::update_prototype(GLFWwindow* window, collider *c1, collider *c2){
   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, bg);
      glfwSetKeyCallback(window, Input::key_callback);
      c1->pos = Input::get_mouse_pos(window);
      if (obj1_t == square){
         c1->size = {0.3f, 0.3f};
         c1->radius = 0.0f;
      } else if (obj1_t == circle){
         c1->size = {0.2f, 0.2f};
         c1->radius = c1->size.x/sqr_2;
      }
      c2->pos = {0.0f, 0.0f};
      c2->size = {0.3f, 0.3f};
      c2->radius = 1.0f;

      if (run_prototype(*c1, *c2)){
         obj1.update({c1->pos.x, c1->pos.y, 0.0f}, c1->size);
         obj1.draw(window, obj1.model, glm::mat4(1.0f), blue);

         obj2.update({c2->pos.x, c2->pos.y, 0.0f}, c2->size);
         obj2.draw(window, obj2.model, glm::mat4(1.0f), blue);
      } else {
         obj1.update({c1->pos.x, c1->pos.y, 0.0f}, c1->size);
         obj1.draw(window, obj1.model, glm::mat4(1.0f), blue);

         obj2.update({c2->pos.x, c2->pos.y, 0.0f}, c2->size);
         obj2.draw(window, obj2.model, glm::mat4(1.0f), red);
      }
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
}

bool Collision_prototype::run_prototype(collider c1, collider c2){
   switch (type){
      case AABB_AABB:
         return AABB_collision(c1, c2);
      case AABB_circle:
         return circle_collision(c1, c2);

   }
   return 0;
}

