#include "game.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include "utils.h"


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
   glfwSetFramebufferSizeCallback(window, frame_buffer_size);
   utils::log("init window");
   return window;
}

float get_deltatime(float *last_time){
   float time, deltatime;
   time = glfwGetTime();
   deltatime = time - *last_time;
   *last_time= time;
   return deltatime;
}
void shut_down(GLFWwindow *window){
   utils::log("shut down");
   glfwDestroyWindow(window);
   glfwTerminate();
}

void frame_buffer_size(GLFWwindow *window, int width, int height){
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

   bool is_relesed(GLFWwindow* window, int key){
      return glfwGetKey(window, key) == GLFW_RELEASE;
   }
   bool is_pressed(GLFWwindow* window, int key){
      return glfwGetKey(window, key) == GLFW_PRESS;
   }
   glm::vec2 get_mouse_pos(GLFWwindow *window){
      window_size sz;
      float w, h;
      double x,y;
      sz = get_window_size(window);
      glfwGetCursorPos(window, &x, &y);
      x = 2*(x/sz.width)-1;
      y = -2*(y/sz.height)+1;
      return {x, y};
   }
};
