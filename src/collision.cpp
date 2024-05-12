#include "collision.h"

bool AABB_collision(TEST_obj obj, glm::vec2 user_size, glm::vec2 pos){
   bool x = pos.x < obj.pos.x + obj.size.x && pos.x + user_size.x > obj.pos.x; 
   bool y = pos.y < obj.pos.y + obj.size.y && pos.y + user_size.y > obj.pos.y; 
   return x && y;
}

