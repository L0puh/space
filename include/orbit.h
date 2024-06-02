/*******************************************

            ORBIT PROTOTYPE

*******************************************/
#ifndef ORBIT_H
#define ORBIT_H

#include "collision.h"
#include "game.h"
#include "state.h"
#include <cstring>
#include <ctime>
#include <vector>

#define MAX_ORBIT 1000

class Map;
class Planet;
struct planet_object;
namespace utils{
   void error(std::string, std::string);
}

struct planet_object{
   float radius;
   glm::vec2 pos;
   glm::vec2 velocity = {0.0f, 0.0f};
   float mass;
   glm::vec2 size;
   float distance_to_center;
   std::vector<glm::vec2> orbit;
};

namespace orbit {
   void draw_planets(std::vector<planet_object> planets, size_t amount, Planet *planet, Object *dot);
   void draw_orbit(std::vector<glm::vec2> orbit, Object *dot);
   void update_plantes(planet_object *p, std::vector<planet_object> planets, size_t amount, glm::vec2 center);
   bool check_collisions(std::vector<planet_object>, collider user, size_t amount);
   void run_orbit_prototype();
};

class Planet: public Object{
   public:
      Planet(std::string src_vertex, std::string src_fragment, Texture *tex_sheet, Texture_sheet coord): 
         Object(src_vertex, src_fragment, tex_sheet, coord){ radius = 10.0f;}
   public:
      uint ID;
      float radius;
      float mass;
      glm::vec2 velocity;
      float distance_to_center = 0;
};

class Map{
   public:
      Map(std::vector<collider> *objs, size_t amount, Planet *planet, glm::vec2 galaxy_center={0.0f, 0.0f});
   private:
      size_t amount_planets = 4; 
      Planet *planet;
      glm::vec2 galaxy_center;
      Object dot;
      std::vector<planet_object> planets;
      int seed = static_cast <unsigned> (time(0));
   public:
      static boarder set_boarders(glm::vec2);
      planet_object get_planet(size_t index) {
         if (index >= planets.size()) 
            utils::error("out of range", "Map");
         return planets.at(index);
      }
      void draw_planets();
      void update(std::vector<collider> *objs){
         for (int i = 1; i < amount_planets; i++)
            orbit::update_plantes(&planets[i], planets, amount_planets, galaxy_center);

         for (int i = 0; i < amount_planets; i++)
            objs->at(i) = {planets[i].pos, planets[i].size, planets[i].radius};
      }
};

class Galaxy{
   public: 
      Galaxy(Object *star, glm::vec2 center_pos, float scale): star(star), center_pos(center_pos), scale(scale){}
   public:
      Object *star;
      bool is_star=false;
      float offset_up=0.0f;
      glm::vec2 center_pos;
      float offset_right=0.0f;
      uint32_t n_seed = 0, seed = random_int(1, 100);
      float scale = 10.0f;
      double rnd_double(double min, double max){
         return ((double)rnd()/(double)(0x7FFFFFFF)) * (max-min) + min;
      }
      int rnd_int(int min, int max) {
         return (rnd() % (max - min)) + min;
      }
      uint32_t rnd(){
         n_seed += 0xe120fc15;
         uint64_t tmp;
         tmp = (uint64_t)n_seed * 0x4a39b70d;
         uint32_t m1 = (tmp >> 32) ^ tmp;
         tmp = (uint64_t)m1 * 0x12fad5c9;
         uint32_t m2 = (tmp >> 32) ^ tmp;
         return m2;
      }
      void generate_objs(Object &obj, float amount, object_type type);
      void generate_galaxy_procedural();
      bool get_star(float x, float y);
      bool is_out(User *user){
         return AABB_collision({center_pos, {2*scale, 2*scale}, scale/2}, {user->pos, user->size, 1.0f});
      }
      void generate_galaxy_sphere(int amount, std::vector<glm::vec2>*);
      void draw_galaxy_sphere(std::vector<glm::vec2>);
      
      void draw_stars();
};


#endif
