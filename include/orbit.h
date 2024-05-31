/*******************************************

            ORBIT PROTOTYPE

*******************************************/
#ifndef ORBIT_H
#define ORBIT_H

#include "collision.h"
#include "game.h"
#include <ctime>
#include <vector>

class Map;
class Planet;
struct planet_object;

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
   void update_plantes(planet_object *p, std::vector<planet_object> planets, size_t amount);
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
      Map(std::vector<collider> *objs, size_t amount);
   private:
      size_t amount_planets = 4; 
      Planet planet;
      Object dot;
      std::vector<planet_object> planets;
      int seed = static_cast <unsigned> (time(0));
   public:
      static boarder set_boarders(glm::vec2);
      void draw_planets();
      void update(std::vector<collider> *objs){
         for (int i = 1; i < amount_planets; i++)
            orbit::update_plantes(&planets[i], planets, amount_planets);

         for (int i = 0; i < amount_planets; i++)
            objs->at(i) = {planets[i].pos, planets[i].size, planets[i].radius};
      }
};

class Galaxy{
   public: 
      Galaxy(Object *star): star(star){}
   public:
      Object *star;
      bool is_star=false;
      float offset_up=0.0f;
      float offset_right=0.0f;
      uint32_t n_seed = 0, seed = random_int(1, 100);
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

      void generate_galaxy_sphere(int scale, int amount, std::vector<glm::vec2>*);
      void draw_galaxy_sphere(std::vector<glm::vec2>, glm::vec2 center_pos);
      
      void draw_stars();
};


#endif
