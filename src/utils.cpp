#include "utils.h"
#include "imgui/imgui.h"
#include <string>

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
};
