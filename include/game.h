#ifndef GAME_H
#define GAME_H

#include <glm/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <vector>

#define LEN(n) sizeof(n)/sizeof(n[0])

class Object;
struct TEST_obj{
      glm::vec2 pos;
      glm::vec2 size;
};

/*********************************************************/
/*                      DATA                             */


const float data_triangle[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const float data_dot[] = {
    0.0f, 0.0f, 0.0f
};
const float data_square[] = {
   // position          texture 
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,    // top right
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // top left 
};
const uint indices_square[] = {
    0, 1, 2,            // first triangle
    0, 2, 3             // second triangle
};


/*********************************************************/
/*                      COLORS                           */

static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat bg[] = {0.07f, 0.08f, 0.08f, 1.0f};

/*********************************************************/
/*                      ENGINE                           */

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

enum Image {
   LINES,
   NONE,
   PNG,
   JPG,
};

class Texture {
   public:
      Texture(std::string filename, Image image_type): filename(filename), type(image_type){}
      ~Texture() {delete_texture();}
   public:
      const uint get_texture(){ return texture_id;}
      void load_texture();
      void delete_texture();
      void use();
      void unuse();
   private:
      int width, height, bit_depth, type;
      std::string filename;
      uint texture_id;
};


class Camera {
   public:
      float speed = 1.2, rotation = 0.0f;
      float map_offset = 2.8f;
      glm::vec3 pos = glm::vec3(0.0f); 
      glm::mat4 view = glm::mat4(1.0f);
      GLFWwindow *window;
   public: 
      void update();
      void set_window(GLFWwindow *win) {window = win;}
      void set_position(glm::vec3 camera_pos) { pos = camera_pos; }
      bool check_boarder(glm::vec2 map_size, glm::vec2 user_size, float x, float y);
      bool AABB_collision(TEST_obj obj, glm::vec2 user_size, glm::vec2 pos);
      void get_movement(GLFWwindow* window, float deltatime, 
            glm::vec2 map_sz, glm::vec2 user_sz, std::vector<TEST_obj>);
      bool check_collisions(std::vector<TEST_obj> objs, glm::vec2 user_size, glm::vec2 pos);
      //void zoom(float x); TODO
};


/*********************************************************/
/*                      OBJECTS                          */

class Object {
   public:
      Object(std::string src_vertex, std::string src_fragment, std::string src_texture, Image img_type);
      Object(std::string src_vertex, std::string src_fragment);
      ~Object();
      glm::mat4 model = glm::mat4(1.0f);
      glm::vec2 size = glm::vec3(0.0f, 0.0f, 0.0f);
      glm::vec3 pos = glm::vec3(0.0f);
      Shader shader;
      Image tex_type;
      Texture texture;
      Vertex vertex;
   public:
      void update() { model = glm::mat4(1.0f); }
      static void scale_object(GLFWwindow *window, glm::mat4 *model, glm::vec2 scaler);
      static void translate_object(GLFWwindow *window, glm::mat4 *model, glm::vec2 pos);
      static void rotate_object(GLFWwindow *window, glm::mat4 *model, float angle, glm::vec3 pos);
      void scale_object(GLFWwindow *window, glm::vec2 scaler);
      void translate_object(GLFWwindow *window, glm::vec2 pos);
      void rotate_object(GLFWwindow *window, float angle, glm::vec3 pos);
      void draw(GLFWwindow* window, glm::mat4 &model, glm::mat4 view);
};

class Planet: public Object{
   public:
      Planet(std::string src_vertex, std::string src_fragment, std::string src_texture, Image img_type, float p_radius): 
         Object(src_vertex, src_fragment, src_texture, img_type){ radius = p_radius;}
    public:
      uint ID;
      float radius;

};

/* class Hole: public Object { */
/* }; */

/* class Asteroid: public Object { */
/* }; */

class User: public Object {
   public: 
      int HP = 100, EXP = 0;

   public:
      User(std::string src_vertex, std::string src_fragment, std::string src_texture, Image img_type):
      Object(src_vertex, src_fragment, src_texture, img_type){};
};


#endif 
