/*******************************************

            ORBIT PROTOTYPE

*******************************************/
#ifndef ORBIT_H
#define ORBIT_H

#include "collision.h"
#include "game.h"
#include <vector>

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

class Map{
   public:
      Map(std::vector<collider> *objs, size_t amount);
   private:
      size_t amount_planets = 4; 
      Planet planet;
      Object dot;
      std::vector<planet_object> planets;
   public:
      boarder set_boarders(glm::vec2);
      void generate_objs(Object &obj, float amount, object_type type);
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
