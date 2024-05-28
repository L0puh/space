#include "game.h"
#include "orbit.h"
#include "state.h"
#include <vector>

boarder Map::set_boarders(glm::vec2 pos){
   boarder cur_boarder = global_states.cur_boarder;
   float map_offset = global_states.camera->map_offset;
   if (pos.y+global_states.zoom >= cur_boarder.max_y || pos.x+global_states.zoom >= cur_boarder.max_x) {
      cur_boarder.prev_x = cur_boarder.max_x;
      cur_boarder.prev_y = cur_boarder.max_y;
      cur_boarder.max_y = pos.y+map_offset;
      cur_boarder.min_x = pos.x-map_offset;
      cur_boarder.max_x = pos.x+map_offset;
      cur_boarder.min_y = pos.y-map_offset;
   }
   if (pos.y-global_states.zoom <= cur_boarder.min_y || pos.x-global_states.zoom <= cur_boarder.min_x) {
      cur_boarder.max_y = pos.y-map_offset;
      cur_boarder.min_x = pos.x+map_offset;
      cur_boarder.max_x = pos.x-map_offset;
      cur_boarder.min_y = pos.y+map_offset;
   }
   return cur_boarder;
}

void Map::generate_objs(Object &obj, float amount, object_type type){
   boarder cur_boarder;
   cur_boarder = set_boarders(global_states.camera->pos);
   global_states.cur_boarder = cur_boarder;
   srand(100);
   for ( float i=cur_boarder.min_x; i <= cur_boarder.max_x; i+=amount){
      for (float j=cur_boarder.min_y; j <= cur_boarder.max_y; j+=amount){
         bool is_star;
         if (type == object_type::dot) is_star = rand() % 256 < 32;
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

Map::Map(std::vector<collider> *objs, size_t amount_planets): 
   amount_planets(amount_planets),
   planet("../shaders/user.vert", "../shaders/user.frag", "../textures/planet.png", PNG, 5.0f),
   dot("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE),
   planets(amount_planets)
{
   
   //FIXME:
   planets[0] = {150.f, {0.0f, 0.0f}, {0.0f, 0.0f},    155.2f, {12.2, 12.2}, 2.0f};
   planets[1] = {10.0f, {3.7f, 6.7f}, {12.0f, 12.0f},  33.0f,  {4.1, 4.1},   2.0f};
   planets[2] = {20.0f, {4.8f, 5.8f}, {-45.0f, -45.0f},22.2f,  {3.2, 3.2},   2.0f};
   planets[3] = {29.0f, {6.9f, 6.9f}, {67.0f, 67.0f},  22.2f,  {4.2, 4.2},   2.0f};

   for (int i = 0; i < amount_planets; i++){
      objs->at(i) = {planets[i].pos, planets[i].size, planets[i].radius};
   }
}

void Map::draw_planets(){
   orbit::draw_planets(planets, amount_planets, &planet, &dot);
}

void Map::draw_stars(){
   generate_objs(dot, global_states.stars_amount, object_type::dot);
}
