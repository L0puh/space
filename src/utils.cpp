#include "collision.h"
#include "utils.h"
#include "imgui/imgui.h"
#include "state.h"

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
                                                               
         ImGui::SliderFloat("SPEED", &camera->speed, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("MAP OFFSET(boarder)", &camera->map_offset, 0, 10.0f, "%.5f", 0);
         ImGui::SliderFloat("ROTATION", &camera->rotation, 0, 10.0f, "%.4f", 0);
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
};
