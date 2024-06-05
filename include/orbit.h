/*******************************************

            ORBIT PROTOTYPE

*******************************************/
#ifndef ORBIT_H
#define ORBIT_H

#include "collision.h"
#include "game.h"
#include <cstring>
#include <ctime>
#include <vector>

#define MAX_ORBIT 1000

class Planet;
class Galaxy;
struct planet_object;
namespace utils{
   void error(std::string, std::string);
}

struct galaxy_object {
   int seed;
   glm::vec2 center_pos;
   float scale;
   int amount_planets;
   int amount_black_holes;
   int amount_stars;
   std::vector<planet_object> planets;
   std::vector<black_hole_object> black_holes;
   std::vector<glm::vec2> stars;
   std::vector<collider> objects;
};
struct black_hole_object {
   glm::vec2 pos;
   glm::vec2 to;
   glm::vec2 size;
   bool is_deadly;
};

struct planet_object{
   float radius;
   float speed=1.0f;
   glm::vec2 pos;
   glm::vec2 direction = {0.0f, 0.0f};
   float mass;
   glm::vec2 size;
   float distance_to_center;
   std::vector<glm::vec2> orbit;
};

namespace orbit {
   void generate_galaxies(int amount, std::vector<int> *seeds, std::vector<galaxy_object> *galaxies);
   void draw_galaxies(int amount, std::vector<galaxy_object> *galaxies, Galaxy*, Black_hole*, std::vector<glm::vec2>*);
   void update_galaxies(int amount, std::vector<galaxy_object>* galaxies, Galaxy* g, std::vector<glm::vec2>*);
   void draw_planets(std::vector<planet_object> *planets, size_t amount, std::vector<glm::vec2>*, Planet *planet, Object *dot, Galaxy*);
   void draw_planets(std::vector<planet_object> *planets, size_t amount, Planet *planet, Object *dot, Galaxy*);
   void draw_orbit(std::vector<glm::vec2> orbit, Object *dot);
   void update_planets(planet_object *p, std::vector<planet_object> planets, size_t amount, glm::vec2 center, float, std::vector<glm::vec2>*);
   bool check_collisions(std::vector<planet_object>, collider user, size_t amount);
   void run_orbit_prototype(Planet*, Object*);
};

class Planet: public Object{
   public:
      Planet(std::string src_vertex, std::string src_fragment, Texture *tex_sheet, Texture_sheet coord={0, 3}): 
         Object(src_vertex, src_fragment, tex_sheet, {0, 3}){ radius = 10.0f;}
   public:
      uint ID;
      float radius;
      float mass;
      glm::vec2 direction;
      float distance_to_center = 0;
};


class Galaxy{
   public: 
      Galaxy(Object *star, Planet* planet): star(star), planet(planet) {}
      void set_data(galaxy_object *x) { 
         data = x; 
         data->objects.resize(data->amount_planets);
         data->planets.resize(data->amount_planets);
         data->black_holes.resize(data->amount_black_holes);
         data->stars.resize(data->amount_stars);
      }
   public:
      bool is_star=false;
      uint32_t prosedural_seed = 0;
      galaxy_object *data;
      Planet *planet;
      Object *star;
   public:
      //GENERATION 
      double rnd_double(double min, double max){ return ((double)rnd()/(double)(0x7FFFFFFF)) * (max-min) + min;}
      int rnd_int(int min, int max) { return (rnd() % (max - min)) + min;}
      uint32_t rnd();
      void generate_objs(Object &obj, float amount, object_type type);
      void generate_galaxy_procedural();
      void generate_galaxy_sphere(int amount, std::vector<glm::vec2>*);
      void generate_black_holes(int amount, std::vector<black_hole_object>*);
   
   public:
      void init_map(std::vector<collider> *objects);
      bool get_star(float x, float y);
      planet_object get_planet(size_t index);
      void draw_galaxy_sphere(std::vector<glm::vec2>);
      static boarder set_boarders(glm::vec2);
      void update(std::vector<collider>*, std::vector<glm::vec2>* );
      void draw_stars();
      void draw_planets(std::vector<glm::vec2>* orbits);
      void draw_planets();
      void draw_black_holes(int amount, std::vector<black_hole_object> &black_holes, Black_hole* bh);
      void collide_black_holes(std::vector<black_hole_object>, size_t, Black_hole*);
      bool is_out(User*);
      bool is_out(collider);
};


#endif
