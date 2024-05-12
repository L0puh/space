#include "collision.h"
#include "game.h"

bool AABB_collision(collider obj, collider obj2){
   bool x = obj2.pos.x < obj.pos.x + obj.size.x && obj2.pos.x + obj2.size.x > obj.pos.x; 
   bool y = obj2.pos.y < obj.pos.y + obj.size.y && obj2.pos.y + obj2.size.y > obj.pos.y; 
   return x && y;
}

bool circle_collision(collider obj, collider obj2){
   //TODO
   return 1;
}
void Collision_prototype::update_prototype(GLFWwindow* window, collider *c1, collider *c2){
   while (!glfwWindowShouldClose(window)){
      glClearBufferfv(GL_COLOR, 0, bg);
      glfwSetKeyCallback(window, Input::key_callback);
      c1->pos = Input::get_mouse_pos(window);
      if (obj1_t == square){
         c1->size = {0.3f, 0.3f};
         c1->radius = 0.0f;
      }

      c2->pos = {0.0f, 0.0f};
      c2->size = {0.3f, 0.3f};
      c2->radius = 1.0f;

      if (run_prototype(*c1, *c2)){
         obj1.update({c1->pos.x, c1->pos.y, 0.0f}, c1->size);
         obj1.draw(window, obj1.model, glm::mat4(1.0f), grey);

         obj2.update({c2->pos.x, c2->pos.y, 0.0f}, c2->size);
         obj2.draw(window, obj2.model, glm::mat4(1.0f), blue);
      } else {
         obj1.update({c1->pos.x, c1->pos.y, 0.0f}, c1->size);
         obj1.draw(window, obj1.model, glm::mat4(1.0f), grey);

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
        break;
   }
   return 0;
}

