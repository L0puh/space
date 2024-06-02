#include "collision.h"
#include "utils.h"
#include "imgui/imgui.h"
#include "state.h"
#include <string>

namespace utils {
   void init_debug_console(){
      ImGui::CreateContext();
      ImGuiIO& IO = ImGui::GetIO();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(global_states.window, true);
      ImGui_ImplOpenGL3_Init();
   }
   void debug_new_frame(){
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
   }
   void debug_console_render(){
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   }
   void debug_console(){
      Camera *camera = global_states.camera;
      User *user = global_states.user;
      ImGui::Begin("console", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::Text("USER:");
         ImGui::Text("x: %.4f, y: %.4f\nrotation: %.4f", 
                                       camera->pos.y,
                                       camera->pos.x,
                                       camera->rotation);
         ImGui::Text("sin: %.4f, cos: %.4f", 
                                       sin(camera->rotation), 
                                       cos(camera->rotation));
                                                               
         ImGui::SliderFloat("SPEED", &camera->speed, 0, 20.0f, "%.7f", 0);
         ImGui::SliderFloat("MAP OFFSET(boarder)", &camera->map_offset, 0, 50.0f, "%.8f", 0);
         ImGui::SliderFloat("ROTATION", &camera->rotation, 0, 10.0f, "%.4f", 0);
         ImGui::SliderFloat("ZOOM", &global_states.zoom, 1.0f, 4000.0f, "%.4f", 0);
         ImGui::SliderFloat("GRAVITY", &global_states.gravity, 1.0f, 20.0f, "%.4f", 0);
         ImGui::SliderFloat("STARS", &global_states.stars_amount, 0.00001f, 1.0f, "%.9f", 0);
         ImGui::SliderFloat("TIMESTAMP", &global_states.timestep, 0.00001f, 1.0f, "%.9f", 0);
         ImGui::SliderInt("HP", &user->HP, 0, 100, "%d", 0);
         ImGui::SliderInt("EXP", &user->EXP, 0, 100, "%d", 0);
      }
      global_states.camera = camera;
      global_states.user   = user;
      ImGui::End();
   }
   void debug_console(collider* c1, collider* c2, Collision_prototype::collision_type *type){
      ImGui::Begin("console", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::Text("collider 1");
         ImGui::SliderFloat("scale: x", &c1->size.x, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("scale: y", &c1->size.y, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("pos: x", &c1->pos.x, 0, 2.0f, "%.4f", 0); ImGui::SameLine();
         ImGui::SliderFloat("pos: y", &c1->pos.y, 0, 2.0f, "%.4f", 0);
         ImGui::Separator();
         ImGui::Text("collider 2");
         
         ImGui::SliderFloat("2. scale: x", &c2->size.x, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("2. scale: y", &c2->size.y, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("2. pos: x", &c2->pos.x, 0, 2.0f, "%.4f", 0); ImGui::SameLine();
         ImGui::SliderFloat("2. pos: y", &c2->pos.y, 0, 2.0f, "%.4f", 0);
         bool circle_AABB, AABB_AABB; 
         ImGui::Checkbox("AABBvsAABB", &global_states.collision_type.AABB_AABB);
         global_states.collision_type.circle_AABB = global_states.collision_type.AABB_AABB ? 0: 1;
      }
      ImGui::End();
   }
   void debug_console(std::vector<planet_object> *planets){
      ImGui::Begin("console", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::SliderFloat("ZOOM", &global_states.zoom, 1.0f, 10.0f, "%.4f", 0);
         ImGui::SliderFloat("GRAVITY", &global_states.gravity, 1.0f, 20.0f, "%.4f", 0);
         ImGui::SliderFloat("TIMESTAMP", &global_states.timestep, 0.00001f, 1.0f, "%.9f", 0);
         for (int i=1; i < planets->size(); i++){
            ImGui::Text("PL %d\n", i);
            std::string name_mass = "MASS" + std::to_string(i),
                        name_dist = "DIST" + std::to_string(i),
                        name_size_x = "SIZEx" + std::to_string(i),
                        name_size_y = "SIZEy" + std::to_string(i),
                        name_vel_x  = "VELx" + std::to_string(i),
                        name_vel_y = "VELy" + std::to_string(i);

            ImGui::SliderFloat(name_mass.c_str(), &(planets->at(i).mass), 0, 100.0f, "%.4f", 0); 
            ImGui::SliderFloat(name_dist.c_str(), &(planets->at(i).distance_to_center), 0, 100.0f, "%.4f", 0); 
            ImGui::SliderFloat(name_size_x.c_str(), &(planets->at(i).size.x), 0, 2.0f, "%.8f", 0); 
            ImGui::SliderFloat(name_size_y.c_str(), &(planets->at(i).size.y), 0, 2.0f, "%.8f", 0); 
            ImGui::SliderFloat(name_vel_x.c_str(), &(planets->at(i).velocity.x), -10, 10.0f, "%.9f", 0); 
            ImGui::SliderFloat(name_vel_y.c_str(), &(planets->at(i).velocity.y), -10, 10.0f, "%.9f", 0);
            ImGui::Separator();
         }
      }
      ImGui::End();
   }
};
