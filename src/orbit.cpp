#include "orbit.h"
#include "game.h"
#include "state.h"
#include "collision.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>


namespace orbit {
   void update_plantes(planet_object *p, std::vector<planet_object> planets, size_t amount){
      p->pos.x = sin(glfwGetTime()*global_states.timestep) / p->mass * p->velocity.y * p->size.x * global_states.gravity;
      p->pos.y = cos(glfwGetTime()*global_states.timestep) / p->mass * p->velocity.x * p->size.y * global_states.gravity;
      p->orbit.push_back(p->pos);
   }

   bool check_collisions(std::vector<planet_object> planets, collider user, size_t amount){
      for (int i=0; i < amount; i++){
         if (circle_collision({planets[i].pos, planets[i].size, planets[i].radius}, user))
               return true;
      }
      return false;
   }

   void draw_planets(std::vector<planet_object> planets, size_t amount, Planet *planet, Object *dot){
      for (int i=0; i < amount; i++){
         draw_orbit(planets[i].orbit, dot);
         planet->update();
         planet->translate_object(planets[i].pos - glm::vec2(global_states.camera->pos.x, global_states.camera->pos.y));
         planet->scale_object(planets[i].size);
         planet->draw(planet->model, global_states.camera->view);
      }
   }

   void draw_orbit(std::vector<glm::vec2> orbit, Object *dot){
      for (int i = 0; i < orbit.size(); i++){
         dot->update();
         dot->translate_object(orbit[i] - glm::vec2(global_states.camera->pos.x, global_states.camera->pos.y));
         dot->draw(dot->model, global_states.camera->view, red);
      }
   }

   void run_orbit_prototype(){
      Planet planet("../shaders/user.vert", "../shaders/user.frag", 
                     "../textures/planet.png", PNG, 5.0f);
      Object dot("../shaders/standard.vert", "../shaders/standard.frag", Image::NONE); 
      int amount_planets = 4;
      std::vector<planet_object> planets(amount_planets);
      planets[0] = {50.0f, {0.0f, 0.0f}, {0.2f, 0.2f}, 5.2f, {0.2, 0.2}, 0.0f};
      planets[1] = {10.0f, {0.4f, 0.4f}, {2.0f, 2.0f}, 4.2f, {0.1, 0.1}, 1.0f};
      planets[2] = {30.0f, {0.8f, 0.8f}, {-3.0f, -3.0f}, 4.2f, {0.2, 0.2}, 1.0f};
      planets[3] = {39.0f, {0.9f, 0.9f}, {4.0f, -5.0f}, 4.2f, {0.2, 0.2}, 1.0f};
      utils::log("DONE PLANET INIT");
      while (!glfwWindowShouldClose(global_states.window)){
         utils::debug_new_frame();
         glClearBufferfv(GL_COLOR, 0, bg_color);
         glfwSetKeyCallback(global_states.window, Input::key_callback);

         utils::debug_console(&planets);
     
         for (int i = 1; i < amount_planets; i++)
            update_plantes(&planets[i], planets, sizeof(planets));
         draw_planets(planets, amount_planets, &planet, &dot);

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
   n_seed = ((int)x & 0xFFFF) << 16 | ((int)y & 0xFFFF);
   return rnd_int(0, 20) == 1;
}
void Galaxy::generate_galaxy_sphere(int scale, int amount, std::vector<glm::vec2> *stars){ // sphere
   srand(seed);
   for (int i=0; i < amount; i++){
      float distance = random_float(1.0f, scale);
      float angle = random_float(1.0f, scale) * 2.f * glm::pi<float>();
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
      star->translate_object(stars[i] - glm::vec2(global_states.camera->pos.x, global_states.camera->pos.y));
      star->draw(star->model, global_states.camera->view, white);
   }
}
void Galaxy::generate_objs(Object &obj, float amount, object_type type){
   boarder cur_boarder;
   cur_boarder = Map::set_boarders(global_states.camera->pos);
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
