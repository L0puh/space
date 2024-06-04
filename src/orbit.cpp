#include "orbit.h"
#include "game.h"
#include "state.h"
#include "collision.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <map>
#include <vector>


namespace orbit {
   void generate_galaxies(int amount, std::vector<int> *seeds, std::vector<galaxy_object> *galaxies){
      srand(1000);
      if (seeds->size() != amount) seeds->resize(amount);
      std::map<int, bool> m; 
      for (int i = 0; i < amount; i++){
         int r = random_int(10, 10000);
         while (m[r] == 1) r = random_int(10, 10000);
         seeds->at(i) = r;
         m[r] = 1;
         galaxies->at(i).seed = r;
         galaxies->at(i).center_pos = {random_float(1, 4000) - 1000, random_float(1, 4000) - 1000};
         galaxies->at(i).scale = random_float(500, 1000);
         galaxies->at(i).amount_stars = galaxies->at(i).scale * 2;
         galaxies->at(i).amount_planets = random_int(1, 20);
         galaxies->at(i).amount_black_holes = galaxies->at(i).amount_planets * 2;
      }
   }
   void update_galaxies(int amount, std::vector<galaxy_object>* galaxies, Galaxy* g){
      for (int i = 0; i < amount; i++){
         g->set_data(&galaxies->at(i));
         g->generate_galaxy_sphere(galaxies->at(i).amount_stars, &galaxies->at(i).stars);
         g->init_map(&galaxies->at(i).objects);
         g->update(&galaxies->at(i).objects);
      }
   }
   void draw_galaxies(int amount, std::vector<galaxy_object> *galaxies, Galaxy* g, Black_hole *h){
      for (int i = 0; i < amount; i++){
         g->set_data(&galaxies->at(i));
         g->collide_black_holes(galaxies->at(i).black_holes, galaxies->at(i).amount_black_holes, h);
         g->generate_black_holes(galaxies->at(i).amount_black_holes, &galaxies->at(i).black_holes);
         g->draw_galaxy_sphere(galaxies->at(i).stars);
         g->draw_black_holes(galaxies->at(i).amount_black_holes, galaxies->at(i).black_holes, h);
         g->draw_planets();
      }
   }
   void update_plantes(planet_object *p, std::vector<planet_object> planets, size_t amount, glm::vec2 center, float scale){
      p->pos.x = sin((glfwGetTime()*global_states.timestep*p->speed)/p->direction.x);
      p->pos.y = cos((glfwGetTime()*global_states.timestep*p->speed)/p->direction.y);
      p->pos.x*=(scale*p->mass/global_states.gravity);
      p->pos.y*=(scale*p->mass/global_states.gravity);

      p->pos.x += center.x; 
      p->pos.y += center.y; 
      if (p->orbit.size()*p->speed < MAX_ORBIT*p->radius)
         p->orbit.push_back(p->pos);
   }

   bool check_collisions(std::vector<planet_object> planets, collider user, size_t amount){
      for (int i=0; i < amount; i++){
         if (circle_collision({planets[i].pos, planets[i].size, planets[i].radius}, user))
               return true;
      }
      return false;
   }

   void draw_planets(std::vector<planet_object> planets, size_t amount, Planet *planet, Object *dot, Galaxy *galaxy){
      collider sun = {planets[0].pos, planets[0].size, planets[0].radius};
      
      for (int i=0; i < amount; i++){
         if (galaxy != nullptr){
            if(galaxy->is_out({planets[i].pos, {1.0f, 1.0f}, planets[i].radius})){
               planets[i].orbit.clear();
               continue;
             }
         }
         draw_orbit(planets[i].orbit, dot);
         planet->update();
         planet->translate_object(planets[i].pos - glm::vec2(global_states.camera->pos));
         planet->scale_object(planets[i].size);
         planet->draw(planet->model, global_states.camera->view);
      }
   }

   void draw_orbit(std::vector<glm::vec2> orbit, Object *dot){
      for (int i = 0; i < orbit.size(); i++){
         dot->update();
         dot->translate_object(orbit[i] - glm::vec2(global_states.camera->pos));
         dot->draw(dot->model, global_states.camera->view, red);
      }
   }

   void run_orbit_prototype(Planet* planet, Object *dot){

      Texture tex_sheet("../textures/texture_sheet.png", PNG);
      tex_sheet.load_texture();
      int amount_planets = 4;
      float scale = 20.0f;
      std::vector<planet_object> planets(amount_planets);
      planets[0] = {50.0f, 0.0f, {0.0f, 0.0f}, {0.2f, 0.2f}, 5.2f, {0.2, 0.2}, 0.0f};
      planets[1] = {10.0f, 0.2f, {0.4f, 0.4f}, {2.0f, 2.0f}, 4.2f, {0.1, 0.1}, 1.0f};
      planets[2] = {30.0f, 0.6f, {0.8f, 0.8f}, {-3.0f, -3.0f}, 4.2f, {0.2, 0.2}, 1.0f};
      planets[3] = {39.0f, 1.0f, {random_float(1.0f, scale), random_float(1.0f, scale)}, {4.0f, -5.0f}, 4.2f, {0.2, 0.2}, 1.0f};
      utils::log("DONE PLANET INIT");
      while (!glfwWindowShouldClose(global_states.window)){
         utils::debug_new_frame();
         glClearBufferfv(GL_COLOR, 0, bg_color);
         glfwSetKeyCallback(global_states.window, Input::key_callback);

         utils::debug_console(&planets);
     
         for (int i = 1; i < amount_planets; i++)
            update_plantes(&planets[i], planets, sizeof(planets), {0.0f, 0.0f}, 10.f);
         draw_planets(planets, amount_planets, planet, dot, nullptr);

         utils::debug_console_render();
         glfwSwapBuffers(global_states.window);
         glfwPollEvents();
      }
   }
}
void Galaxy::generate_galaxy_procedural(){
   int max = global_states.camera->map_offset; 
   for (float x=0.0f; x < max; x += global_states.stars_amount){
      for (float y=0.0f; y < max; y += global_states.stars_amount){
         float seed1 = global_states.camera->pos.x +  x;
         float seed2 = global_states.camera->pos.y +  y;
         bool is_star = get_star(seed1, seed2);
         if (is_star){
            star->update();
            star->translate_object({x, y});
            star->draw(star->model, global_states.camera->view, white);
         }
      }
   }
}
bool Galaxy::get_star(float x, float y){
   prosedural_seed = ((int)x & 0xFFFF) << 16 | ((int)y & 0xFFFF);
   return rnd_int(0, 20) == 1;
}
planet_object Galaxy::get_planet(size_t index) {
   if (index >= data->amount_planets) 
      utils::error("out of range", "Map");
   return data->planets.at(index);
}
bool Galaxy::is_out(User *user){
   return AABB_collision({data->center_pos, {2*data->scale, 2*data->scale}, data->scale/2}, {user->pos, user->size, 1.0f});
}
bool Galaxy::is_out(collider col){
   return AABB_collision({data->center_pos, {2*data->scale, 2*data->scale}, data->scale/2}, col);;
}
void Galaxy::update(std::vector<collider> *objs){
   for (int i = 1; i < data->amount_planets; i++)
      orbit::update_plantes(&data->planets[i], data->planets, data->amount_planets, data->center_pos, data->scale);

   for (int i = 0; i < data->amount_planets; i++)
      objs->at(i) = {data->planets[i].pos, data->planets[i].size, data->planets[i].radius};
}
void Galaxy::generate_black_holes(int amount, std::vector<black_hole_object> *black_holes){
   srand(data->seed);
   for (int i=0; i < amount; i++){
      float pos_x = random_float(1.0f, data->scale);
      float pos_y = random_float(1.0f, data->scale);
      float pos_to_x = random_float(1.0f, data->scale+100.f)-60.f;
      float pos_to_y = random_float(1.0f, data->scale+100.f)-60.f;
      float size = random_float(40.0f, 50.0f);
      black_holes->at(i).pos = glm::vec2(pos_x, pos_y) + data->center_pos;
      black_holes->at(i).to = glm::vec2(pos_to_x, pos_to_y) + data->center_pos;
      black_holes->at(i).size = {size, size/2};
      black_holes->at(i).is_deadly = random_int(0, 2);
   }
}

void Galaxy::collide_black_holes(std::vector<black_hole_object> holes, size_t amount, Black_hole* bh){
   for (int i=0; i < amount; i++){
      bh->set_pos(glm::vec3(holes[i].pos, 0.0f)-global_states.camera->pos);
      bh->collide(holes[i], global_states.user->size);  
   }
}
void Galaxy::draw_black_holes(int amount, std::vector<black_hole_object> &black_holes, Black_hole* bh){
   for (int i=0; i<amount; i++){
      bh->update();
      bh->translate_object(black_holes[i].pos-glm::vec2(global_states.camera->pos));
      bh->scale_object(black_holes[i].size);
      bh->draw(bh->model, global_states.camera->view);
   }
}
void Galaxy::generate_galaxy_sphere(int amount, std::vector<glm::vec2> *stars){ // sphere
                                                                                
   srand(data->seed);
   for (int i=0; i < amount; i++){
      float distance = random_float(1.0f, data->scale);
      float angle = random_float(1.0f, data->scale) * 2.f * glm::pi<float>();
      float pos_x = cos(angle) * distance;
      float pos_y = sin(angle) * distance; 
      stars->at(i) = {pos_x, pos_y};
      
   }
}
void Galaxy::draw_stars(){
   generate_objs(*star, global_states.stars_amount, object_type::dot);
}
void Galaxy::draw_galaxy_sphere(std::vector<glm::vec2> stars){
   for (int i=0; i<stars.size();i++) {
      star->update();
      star->translate_object((stars[i]+data->center_pos) - glm::vec2(global_states.camera->pos));
      star->draw(star->model, global_states.camera->view, white);
   }
}
uint32_t Galaxy::rnd(){
   prosedural_seed += 0xe120fc15;
   uint64_t tmp;
   tmp = (uint64_t)prosedural_seed* 0x4a39b70d;
   uint32_t m1 = (tmp >> 32) ^ tmp;
   tmp = (uint64_t)m1 * 0x12fad5c9;
   uint32_t m2 = (tmp >> 32) ^ tmp;
   return m2;
}
void Galaxy::generate_objs(Object &obj, float amount, object_type type){
   boarder cur_boarder;
   cur_boarder = set_boarders(global_states.camera->pos);
   global_states.cur_boarder = cur_boarder;
   srand(data->seed);
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
boarder Galaxy::set_boarders(glm::vec2 pos){
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


void Galaxy::init_map(std::vector<collider> *objects){
   if (data->amount_planets != objects->size() || data->amount_planets == 0 || objects->size() == 0) 
      utils::error("size of planets != size of objects", "map init");
   data->planets[0] = {550.f, 2.0f, data->center_pos, {0.0f, 0.0f}, 15.2f, {20.2, 20.2}, 2.0f}; //SUN
   srand(data->seed);
   for (int i = 1; i < data->amount_planets; i++){
      float vel = (random_float(0, 420)-100);
      float sz = random_float(1, 10);
      data->planets[i] = {sz, 1/random_float(1, 100),
         {random_float(1.0f, data->scale), random_float(1.0f, data->scale)}, //pos
         {vel, vel}, random_float(1, 6),
         {sz, sz}, 10.0f};
   }
   for (int i = 0; i < objects->size(); i++){
      objects->at(i) = {data->planets[i].pos, data->planets[i].size, data->planets[i].radius};
   }
}
void Galaxy::draw_planets(){
   orbit::draw_planets(data->planets, data->amount_planets, planet, star, this);
}


