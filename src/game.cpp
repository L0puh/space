#include "game.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>


GLFWwindow* init_window(const int width, const int height){
   GLFWwindow *window;
   if (!glfwInit()) {
      utils::error("glfw init");
      exit(-1);
      return 0;
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   window = glfwCreateWindow(width, height, "window", 0, 0);
   if (window == NULL) {
      utils::error("window init");
      glfwTerminate();
      return 0;
   }
   glfwMakeContextCurrent(window);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      utils::error("glad load");
      glfwTerminate();
      return 0;
   }
   glfwSetFramebufferSizeCallback(window, frame_butter_size);
   utils::log("init window");
   return window;
}

void shut_down(GLFWwindow *window){
   utils::log("shut down");
   glfwDestroyWindow(window);
   glfwTerminate();
}

void frame_butter_size(GLFWwindow *window, int width, int height){
   utils::log("changed view port");
   glViewport(0, 0, width, height);
}


void GLAPIENTRY message_callback
(GLenum src, GLenum type, GLuint id, GLuint severity,
GLsizei len, const GLchar* msg, const GLvoid* param){
   if (type == GL_DEBUG_TYPE_ERROR){
      utils::error(msg, std::to_string(type));
      exit(-1);
   }

}
std::string load_from_file(const std::string& src){
   std::string res; 
   std::ifstream file;
   file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   try {
      file.open(src.c_str());
      std::stringstream stream;
      stream << file.rdbuf();
      file.close();
      res = stream.str();
   }
   catch(std::ifstream::failure error){
      utils::error(src, error.what());
   }
   return res;
}

namespace Input {
   void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
      switch(key){
         case GLFW_KEY_Q: 
            glfwSetWindowShouldClose(window, true);
            return;
      }
   }

   bool is_pressed(GLFWwindow* window, int key){
      return glfwGetKey(window, key) == GLFW_PRESS;
   }
   void move_object(GLFWwindow* window, direction *move) {
      if (is_pressed(window, GLFW_KEY_A)){
         if (move->right_left - SPEED <= -1) return;
         move->right_left -= SPEED;
      } 
      if (is_pressed(window, GLFW_KEY_D)){
         if (move->right_left + SPEED >= 1) return;

         move->right_left += SPEED;
      }
      if (is_pressed(window, GLFW_KEY_W)){
         if (move->up_down + SPEED >= 1) return;
         move->up_down += SPEED;
      }
      if (is_pressed(window, GLFW_KEY_S)){
         if (move->up_down - SPEED <= -1) return;
         move->up_down -= SPEED;
      }
   }
};