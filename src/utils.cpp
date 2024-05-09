#include "utils.h"

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
         ImGui::SliderFloat("SPEED", &camera->speed, 0, 2.0f, "%.4f", 0);
         ImGui::SliderInt("HP", &user->HP, 0, 100, "%d", 0);
         ImGui::SliderInt("EXP", &user->EXP, 0, 100, "%d", 0);
      }
      ImGui::End();
   }
};
