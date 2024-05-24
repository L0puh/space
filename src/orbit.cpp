#include "orbit.h"
#include "state.h"
#include "utils.h"
#include <cmath>

const float G = 5.0f;
const float timestep = 0.0000005f;

void update_plantes(planet_objects *p, planet_objects planets[], size_t amount){
   float total_force_x, total_force_y;
   total_force_x = total_force_y = 0;

   float dx, dy, g, ang;
   for (int i = 0; i < amount; i++){
      if (p->pos == planets[i].pos) continue;
      planet_objects p2 = planets[i];
      dx = p2.pos.x - p->pos.x;
      dy = p->pos.y - p2.pos.y;
      printf("posx: %.4f posy: %.4f\n", p2.pos.x, p2.pos.y);
      printf("posx: %.4f posy: %.4f\n", p->pos.x, p->pos.y);
      g = G * p2.mass / (dx * dx + dy * dy);
      ang = atan2(dx, dy);
      total_force_x +=  g * cos(ang);
      total_force_y += -g * sin(ang);
      printf("dx: %.4f dy: %.4f ang : %.4f\n", dx, dy, ang);
   }
   p->velocity.x += total_force_x * timestep;
   p->velocity.y += total_force_y * timestep;

   p->pos.x += p->velocity.x * timestep;
   p->pos.y += p->velocity.y * timestep;

   //FIXME:
   if (std::abs(p->pos.x) >= 1.0f){
      p->pos.x = 0.3f;
   }
   if (std::abs(p->pos.y) >= 1.0f){
      p->pos.y = 0.3f;
   }
}

void draw_planets(planet_objects planets[], size_t amount, Planet *planet){
   for (int i=0; i < amount; i++){
      planet->update();
      planet->translate_object(planets[i].pos);
      planet->scale_object(planets[i].size);
      planet->draw(planet->model, glm::mat4(1.0f));
   }
}

void run_orbit_prototype(){
   Planet planet("../shaders/user.vert", "../shaders/user.frag", 
                  "../textures/planet.png", PNG, 5.0f);

   planet_objects planets[2];
   planets[0] = {50.0f, {0.1f, 0.1f}, {0.2f, 0.2f}, 5.2f, {0.2, 0.2}, 0.0f};
   planets[1] = {10.0f, {0.1f, 0.1f}, {1.5f, 1.5f}, 4.2f, {0.1, 0.1}, 1.0f};

   while (!glfwWindowShouldClose(global_states.window)){
      utils::debug_new_frame();
      glClearBufferfv(GL_COLOR, 0, bg);
      glfwSetKeyCallback(global_states.window, Input::key_callback);

      update_plantes(&planets[1], planets, sizeof(planets));

      draw_planets(planets, sizeof(planets), &planet);

      utils::debug_console_render();
      glfwSwapBuffers(global_states.window);
      glfwPollEvents();
   }
}
