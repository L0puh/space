#ifndef GAME_H
#define GAME_H

#include <glm/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>

#define LEN(n) sizeof(n)/sizeof(n[0])

/*********************************************************/
/*                      DATA                             */


const float data_triangle[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const float data_square[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
const uint indices_square[] = {
    0, 1, 3,            // first triangle
    1, 2, 3             // second triangle
};


/*********************************************************/
/*                      COLORS                           */

static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat bg[] = {0.07f, 0.08f, 0.08f, 1.0f};

/*********************************************************/
/*                      ENGINE                           */

struct direction {
   float up_down=0;
   float right_left=0;
};
struct window_size {
   int width;
   int height;
};

inline window_size get_window_size(GLFWwindow *window){
   int width, height;
   glfwGetFramebufferSize(window, &width, &height);
   return {width, height};
}

GLFWwindow* init_window(const int width, const int height);
void shut_down(GLFWwindow *window);
void set_debug_mode();
void frame_butter_size(GLFWwindow *window, int width, int height);
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
     
      void add_attribute(uint id, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset);
      void bind()  { glBindVertexArray(VAO);}
      void unbind(){ glBindVertexArray(0);}

      uint get_VAO(){ return VAO;}
      uint get_VBO(){ return VBO;}
      uint get_EBO(){ return EBO;}

   private: 
      uint VAO, VBO, EBO;

};

namespace Input {
   void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
   bool is_pressed(GLFWwindow* window, int key);
};   

class Texture {
   public:
      Texture();
      ~Texture();
   public:
      const uint get_texture();
      void create_texture(const std::string& src, int type, GLenum mode);
      void delete_texture();
      void use();
      void unuse();
   private:
      uint texture_id;
};


class Camera {
};


/*********************************************************/
/*                      OBJECTS                          */

class Object {
   public:
      Object(std::string src_vertex, std::string src_fragment);
      ~Object();
      Shader shader;
      Texture texture;
      Vertex vertex;
   public:
      static void scale_object(GLFWwindow *window, glm::mat4 *model, glm::vec2 scaler);
      static void translate_object(GLFWwindow *window, glm::mat4 *model, glm::vec2 pos);
      static void rotate_object(GLFWwindow *window, glm::mat4 *model, float angle, glm::vec3 pos);
      void draw(GLFWwindow* window, glm::mat4 &model);
};

class Planet: public Object{
   public:
      Planet(std::string src_vertex, std::string src_fragment) : Object(src_vertex, src_fragment){};
   public:
      uint ID;

};

/* class Hole: public Object { */
/* }; */

/* class Asteroid: public Object { */
/* }; */

class User: public Object {
   public: 
      float speed = 0.03f;
      int HP = 100, EXP = 0;
      direction moving_direction;

   public:
      User(std::string src_vertex, std::string src_fragment) : Object(src_vertex, src_fragment){};
      void get_movement(GLFWwindow* window, glm::mat4* model);
};

#endif 
