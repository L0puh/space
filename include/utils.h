#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include "collision.h"
#include "game.h"
#include "orbit.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

struct collider;
class Collision_prototype;

namespace utils{
   inline void log(std::string a){
      printf("[+]: %s\n",a.c_str());
   }
   inline void log(std::string a, int b){
      printf("[+]: %s %d\n",a.c_str(), b);
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
   void init_debug_console();
   void debug_console();
   void debug_new_frame();
   void debug_console_render();
   void debug_console(collider*, collider*, Collision_prototype::collision_type*);
   void debug_console(std::vector<planet_object> *planets);
};


#endif
