#include "game.h"
#include "state.h"

namespace Map{
   boarder set_boarders(glm::vec2 pos){
      boarder cur_boarder = global_states.cur_boarder;
      float map_offset = global_states.camera->map_offset;
      if (pos.y+1.0f >= cur_boarder.max_y || pos.x+1.0f >= cur_boarder.max_x) {
         cur_boarder.prev_x = cur_boarder.max_x;
         cur_boarder.prev_y = cur_boarder.max_y;
         cur_boarder.max_y = pos.y+map_offset;
         cur_boarder.min_x = pos.x-map_offset;
         cur_boarder.max_x = pos.x+map_offset;
         cur_boarder.min_y = pos.y-map_offset;
      }
      if (pos.y-1.0f <= cur_boarder.min_y || pos.x-1.0f <= cur_boarder.min_x) {
         cur_boarder.max_y = pos.y-map_offset;
         cur_boarder.min_x = pos.x+map_offset;
         cur_boarder.max_x = pos.x-map_offset;
         cur_boarder.min_y = pos.y+map_offset;
      }
      return cur_boarder;
   }

   void generate_objs(Object &obj, float amount, object_type type){
      boarder cur_boarder;
      cur_boarder = set_boarders(global_states.camera->pos);
      global_states.cur_boarder = cur_boarder;
      srand(1000);
      for ( float i=cur_boarder.min_x; i <= cur_boarder.max_x; i+=amount){
         for (float j=cur_boarder.min_y; j <= cur_boarder.max_y; j+=amount){
            bool is_star;
            if (type == dot) is_star = rand() % 256 < 32;
            else is_star = rand() % 1024 < 15;
            if (is_star) {
               obj.update();
               obj.translate_object(glm::vec2(i, j));
               if (type == object_type::dot)
                  obj.draw(obj.model, global_states.camera->view, white);
               else 
                  obj.draw(obj.model, global_states.camera->view);
            }
         }
      }
      global_states.cur_boarder = cur_boarder;
   }
}
