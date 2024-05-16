#include "utils.h"
#include "collision.h"
#include "imgui/imgui.h"

namespace utils {
   void init_debug_console(GLFWwindow *window){
      ImGui::CreateContext();
      ImGuiIO& IO = ImGui::GetIO();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
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
   void debug_console(GLFWwindow* window, User *user, Camera *camera){
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
      ImGui::End();
   }
   void debug_console(GLFWwindow* window, collider* c1, collider* c2, Collision_prototype::collision_type *type){
      ImGui::Begin("console", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {
         ImGui::Text("collider 1");
         ImGui::SliderFloat("scale: x", &c1->size.x, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("scale: y", &c1->size.y, 0, 2.0f, "%.4f", 0);
         float scale = c1->size.x;
         ImGui::SliderFloat("scale: both", &scale, 0, 2.0f, "%.4f", 0);
         c1->size = {scale, scale};
         ImGui::SliderFloat("pos: x", &c1->pos.x, 0, 2.0f, "%.4f", 0); ImGui::SameLine();
         ImGui::SliderFloat("pos: y", &c1->pos.y, 0, 2.0f, "%.4f", 0);
         ImGui::Separator();
         ImGui::Text("collider 2");
         
         scale = c2->size.x;
         ImGui::SliderFloat("scale: x", &c2->size.x, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("scale: y", &c2->size.y, 0, 2.0f, "%.4f", 0);
         ImGui::SliderFloat("scale: both", &scale, 0, 2.0f, "%.4f", 0);
         c2->size = {scale, scale};
         ImGui::SliderFloat("pos: x", &c2->pos.x, 0, 2.0f, "%.4f", 0); ImGui::SameLine();
         ImGui::SliderFloat("pos: y", &c2->pos.y, 0, 2.0f, "%.4f", 0);
         bool AABB_AABB=0, circle_AABB; //FIXME: keep state, create global state 
         ImGui::Checkbox("AABBvsAABB", &AABB_AABB);
         if (AABB_AABB) {
            circle_AABB = 0;
            *type = Collision_prototype::AABB_AABB;
         } else {
            circle_AABB = 1;
            *type = Collision_prototype::AABB_circle;
         }
      }
      ImGui::End();
   }
};
