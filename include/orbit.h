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
      Planet(std::string src_vertex, std::string src_fragment, std::string src_texture, Image img_type, float p_radius): 
         Object(src_vertex, src_fragment, src_texture, img_type){ radius = p_radius;}
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
      float random_float(int start, int scale);
      boarder set_boarders(glm::vec2);
      void generate_objs(Object &obj, float amount, object_type type);
      void generate_galaxy(int scale, int amount, std::vector<glm::vec2>*);
      void draw_galaxy(std::vector<glm::vec2>);
      void draw_stars();
      void draw_planets();
      void update(std::vector<collider> *objs){
         for (int i = 1; i < amount_planets; i++)
            orbit::update_plantes(&planets[i], planets, amount_planets);

         for (int i = 0; i < amount_planets; i++)
            objs->at(i) = {planets[i].pos, planets[i].size, planets[i].radius};
      }
};


#endif
