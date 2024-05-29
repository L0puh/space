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
float Map::random_float(int start, int scale){
   return start + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/scale));
}

void Map::generate_galaxy(int scale, int amount, std::vector<glm::vec2> *stars){
   srand(seed);
   for (int i=0; i < amount; i++){
      float distance = random_float(1.0f, scale);
      float angle = random_float(1.0f, scale) * 2.f * glm::pi<float>();
      float pos_x = cos(angle) * distance;
      float pos_y = sin(angle) * distance;
      stars->at(i) = {pos_x, pos_y};
   }
}

void Map::generate_objs(Object &obj, float amount, object_type type){
   boarder cur_boarder;
   cur_boarder = set_boarders(global_states.camera->pos);
   global_states.cur_boarder = cur_boarder;
   srand(seed);
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
  
   planets[0] = {550.f, {0.0f, 0.0f}, {0.0f, 0.0f}, 15.2f, {12.2, 12.2}, 2.0f};
   srand(seed);
   for (int i = 1; i < amount_planets; i++){
      float vel = random_float(0, 80);
      float sz = random_float(1, 10);
      planets[i] = {random_float(20, 500), 
         {random_float(0, 250), random_float(0, 250)},
         {vel, vel}, random_float(0, 7), 
         {sz, sz}, 10.0f};
   }
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
void Map::draw_galaxy(std::vector<glm::vec2> stars){
   for (int i=0; i<stars.size();i++) {
      dot.update();
      dot.translate_object(stars[i]);
      dot.draw(dot.model, global_states.camera->view, white);
   }
}
