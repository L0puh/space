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
void shut_down(){
   utils::log("shut down");
   glfwDestroyWindow(global_states.window);
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

   bool is_relesed(int key){
      return glfwGetKey(global_states.window, key) == GLFW_RELEASE;
   }
   bool is_pressed(int key){
      return glfwGetKey(global_states.window, key) == GLFW_PRESS;
   }
   glm::vec2 get_mouse_pos(){
      double x,y;
      glfwGetCursorPos(global_states.window, &x, &y);
      x = 2*(x/global_states.w_size.width)-1;
      y = -2*(y/global_states.w_size.height)+1;
      return {x, y};
   }
};

void generate_stars(Object &star, float amount){
   boarder cur_boarder = global_states.cur_boarder;
   glm::vec2 pos = global_states.camera->pos;
   float map_offset = global_states.camera->map_offset;

   if (pos.y+1.0f >= cur_boarder.max_y || pos.x+1.0f >= cur_boarder.max_x) {
      cur_boarder.prev_x = cur_boarder.max_x;
      cur_boarder.prev_y = cur_boarder.max_y;
      cur_boarder.max_y = pos.y+map_offset;
      cur_boarder.min_x = pos.x-map_offset;
      cur_boarder.max_x = pos.x+map_offset;
      cur_boarder.min_y = pos.y-map_offset;
   }
   if (pos.y-1.0f <= cur_boarder.min_y || pos.x-1.0f <= cur_boarder.min_x) {
      cur_boarder.max_y = pos.y-map_offset;
      cur_boarder.min_x = pos.x+map_offset;
      cur_boarder.max_x = pos.x-map_offset;
      cur_boarder.min_y = pos.y+map_offset;
   }
   srand(1000);
   for ( float i=cur_boarder.min_x; i <= cur_boarder.max_x; i+=amount){
      for (float j=cur_boarder.min_y; j <= cur_boarder.max_y; j+=amount){
         bool is_star = rand() % 256 < 32;
         if (is_star) {
            star.update();
            star.translate_object(glm::vec2(i, j));
            star.draw(star.model, global_states.camera->view, white);
         }
      }
   }
   global_states.cur_boarder = cur_boarder;
}

