#ifndef GAME_H
#define GAME_H

#include <glm/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <vector>

class Texture;
class Object;
class Planet;
class Asteroid;
class User;
class Hole;
class Vertex;
class Shader;


/*********************************************************/
/*                      COLORS                           */

static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat bg[] = {0.07f, 0.08f, 0.08f, 1.0f};

/*********************************************************/
/*                      ENGINE                           */

GLFWwindow* init_window(const int width, const int height);
void shut_down(GLFWwindow *window);
void set_debug_mode();
void frame_butter_size(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLAPIENTRY message_callback(GLenum src, GLenum type, GLuint id, GLuint severity,
                              GLsizei len, const GLchar* msg, const GLvoid* param);
std::string load_from_file(const std::string& src);


class Shader {
   public:
      Shader(const std::string& src_vertex, const std::string& src_fragment);
      ~Shader() { delete_shader(); }
   public:
      void create_shader(uint *shader, const std::string& src, GLuint type);
      void delete_shader();
      void use()  { glUseProgram(shader_id);}
      void unuse(){ glUseProgram(0);}
   public:
      const int get_location(const std::string& name);
      const uint get_shader(){return shader_id;}
      void set_float(const std::string& name, float x);
      void set_int(const std::string& name, int x);
      void set_vec3(const std::string& name, glm::vec3 x);
      void set_vec4(const std::string& name, glm::vec4 x);
      void set_matrix4fv(const std::string& name, glm::mat4 x);

   private:
      uint shader_id;
      std::unordered_map<std::string, uint> cached_locations;
};

class Vertex{
   public: 
      Vertex();
      ~Vertex();

   public:
      void draw(GLenum mode, size_t size);
      void draw_buffer(GLenum mode, size_t size);
      void create_VBO(const void* data, size_t size);
      void create_EBO(const void* data, size_t size);
      
      void add_attribute(uint id, GLint size, GLenum type, GLboolean normalized, GLsizei stribe, size_t offset);
      void bind()  { glBindVertexArray(VAO);}
      void unbind(){ glBindVertexArray(0);}

      void add_buffer();
      uint get_VAO(){ return VAO;}
      uint get_VBO(){ return VBO;}
      uint get_EBO(){ return EBO;}

   private: 
      uint VAO, VBO, EBO;

};

class Object {
};

class Planet: public Object {
   public:
      Planet();
      ~Planet();
   public:
      uint ID;
      const std::string src_vertex  ="../shaders/planet.vert", 
                        src_fragment="../shaders/planet.frag";
      Shader shader;
      const float verticies[];
      
   public:
      void set_position(glm::vec3);

};

class Hole: public Object {
};

class Asteroid: public Object {
};

class User: public Object {
};

class Data {
};

 
class Texture {
   public:
      Texture();
      ~Texture();
   public:
      void create_texture(const std::string& src, int type, GLenum mode);
      void delete_texture();
      void use();
      void unuse();
      const uint get_texture();
   private:
      uint texture_id;
};

class Input {
};

class Camera {
};


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
};


#endif 
