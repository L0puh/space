#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include "game.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


namespace utils{
   inline void log(std::string a){
      printf("[+]: %s\n",a.c_str());
   }
   inline void log(std::string a, std::string b){
      printf("[+]: %s: %s\n",a.c_str(), b.c_str());
   }
   inline void error(std::string a){
      printf("[!]: %s\n",a.c_str());
   }
   inline void error(std::string a, std::string b){
      printf("[!]: %s: %s\n",a.c_str(), b.c_str());
   }
   void init_debug_console(GLFWwindow *window);
   void debug_console(GLFWwindow *window, User *user, Camera *camera);
   void debug_new_frame();
   void debug_console_render();
};


#endif
