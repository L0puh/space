#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

/*********************************************************/
/*                      ENGINE                           */

GLFWwindow* init_window(const int width, const int height);
void shut_down(GLFWwindow *window);
void set_debug_mode();
void frame_butter_size(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLAPIENTRY message_callback(GLenum src, GLenum type, GLuint id, GLuint severity,
                              GLsizei len, const GLchar* msg, const GLvoid* param);


class Object {
};

class Data {
};

class Shader {
};
 
class Texture {
};

class Input {
};

class Camera {
};


namespace utils{
   inline void log(std::string a){
      printf("[+]: %s\n",a.c_str());
   }
   inline void error(std::string a){
      printf("[!]: %s\n",a.c_str());
   }
   inline void error(std::string a, std::string b){
      printf("[!]: %s %s\n",a.c_str(), b.c_str());
   }
};


#endif 
