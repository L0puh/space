#include "game.h"
#include "orbit.h"
#include "state.h"
#include <cstddef>
#include <ctime>
#include <vector>

boarder Map::set_boarders(glm::vec2 pos){
   boarder cur_boarder = global_states.cur_boarder;
   float map_offset = global_states.camera->map_offset;
   if (pos.y+global_states.zoom >= cur_boarder.max_y || pos.x+global_states.zoom >= cur_boarder.max_x) {
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


Map::Map(std::vector<collider> *objs, size_t amount_planets, Planet* planet): 
   amount_planets(amount_planets),
   planet(planet),
   dot("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE),
   planets(amount_planets)
{
  
   planets[0] = {550.f, {0.0f, 0.0f}, {0.0f, 0.0f}, 15.2f, {12.2, 12.2}, 2.0f};
   srand(seed);
   for (int i = 1; i < amount_planets; i++){
      float vel = random_float(0, 80);
      float sz = random_float(1, 10);
      planets[i] = {sz, 
         {random_float(0, 250), random_float(0, 250)},
         {vel, vel}, random_float(0, 7), 
         {sz, sz}, 10.0f};
   }
   for (int i = 0; i < amount_planets; i++){
      objs->at(i) = {planets[i].pos, planets[i].size, planets[i].radius};
   }
}

void Map::draw_planets(){
   orbit::draw_planets(planets, amount_planets, planet, &dot);
}

