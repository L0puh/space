#ifndef GAME_H
#define GAME_H

#include "glm/ext/matrix_transform.hpp"
#include <cstring>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <vector>

#define LEN(n) sizeof(n)/sizeof(n[0])

struct collider;
class Object;
class User; 
class Renderer;
class Vertex;
class Shader;
class Texture;
class Camera;

enum object_type {
   circle,
   square,
   dot
};

struct boarder {
   float max_x = 3.0f;
   float min_x = -3.0f;
   float max_y = 3.0f;
   float min_y = -3.0f;
   float prev_x = 3.0f;
   float prev_y = 3.0f;
};

struct Data {
   glm::vec3 pos_tr;
   glm::vec3 pos_br;
   glm::vec3 pos_bl;
   glm::vec3 pos_tl;
   glm::vec2 tex_tr;
   glm::vec2 tex_br;
   glm::vec2 tex_bl;
   glm::vec2 tex_tl;
};

/*********************************************************/
/*                      DATA                             */

const Data data_square{
   .pos_tr{0.5f,  0.5f, 0.0f},
   .pos_br{0.5f, -0.5f, 0.0f},
   .pos_bl{-0.5f, -0.5f, 0.0f}, 
   .pos_tl{-0.5f,  0.5f, 0.0f},
   .tex_tr{1.0f, 1.0f},
   .tex_br{1.0f, 0.0f},
   .tex_bl{0.0f, 0.0f}, 
   .tex_tl{0.0f, 1.0f},
};

const float square_vertices[] = {
   // position          
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, // top left 
};

const float data_triangle[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const float data_dot[] = {
    0.0f, 0.0f, 0.0f
};

const uint indices_square[] = {
    0, 1, 2,            // first triangle
    0, 2, 3             // second triangle
};


/*********************************************************/
/*                      COLORS                           */

const glm::vec3 red = {1.0f, 0.0f, 0.0f};
const glm::vec3 white = {1.0f, 1.0f, 1.0f};
const glm::vec3 blue = {0.0f, 0.0f, 1.0f};
const glm::vec3 black = {0.0f, 0.0f, 0.0f};
const glm::vec3 grey = {0.13f, 0.186f, 0.176f};
const GLfloat bg_color[] = {0.07f, 0.08f, 0.08f, 1.0f};

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
void shut_down();
float random_float(int start, int scale);
int random_int(int start, int scale);
void set_debug_mode();
void frame_buffer_size(GLFWwindow *window, int width, int height);
std::string load_from_file(const std::string& src);
float get_deltatime(float *last_time);
void GLAPIENTRY message_callback(GLenum src, GLenum type, GLuint id, GLuint severity,
                                 GLsizei len, const GLchar* msg, const GLvoid* param);

class Shader {
   public:
      Shader(const std::string& src_vertex, const std::string& src_fragment);
      Shader() {}
      ~Shader() { delete_shader(); }
   public:
      void init_shader(const std::string& src_vertex, const std::string& src_fragment);
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
   void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
   bool is_pressed(int key);
   bool is_relesed(int key);
   glm::vec2 get_mouse_pos();
};   

enum Image {
   LINES,
   NONE,
   PNG,
   JPG,
};

class Texture {
   public:
      Texture() {}
      Texture(std::string filename, Image image_type){init_texture(filename, image_type);}
      ~Texture() {delete_texture();}
   public:
      void init_texture(std::string filename, Image img_type);
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
      float speed = 1.2, rotation_speed = 1.2, rotation = 0.0f;
      float map_offset = 60.0f;
      glm::vec3 pos = glm::vec3(0.0f); 
      glm::vec3 inital_pos = glm::vec3(0.0f, 0.0f, 0.0f);
      glm::mat4 view = glm::mat4(1.0f);
   public: 
      void update();
      void set_position(glm::vec3 camera_pos) { pos = camera_pos; }
      void set_init_position() {
         //FIXME for procedural generation: 
         /* inital_pos = {map_offset/2.0f, map_offset/2.0f, 0.0f}; */
         view = glm::translate(glm::mat4(1.0f), inital_pos);
      }
      void get_movement(float deltatime, glm::vec2 user_sz, std::vector<collider>);
      bool check_collisions(std::vector<collider> objs, glm::vec2 user_size, glm::vec2 pos);
};


/*********************************************************/
/*                      OBJECTS                          */

class Object {
   public:
      Object(object_type);
      Object(std::string src_vertex, std::string src_fragment, std::string src_texture, Image img_type, Data data=data_square);
      Object(std::string src_vertex, std::string src_fragment, Image img_type);
      ~Object();
      glm::mat4 model = glm::mat4(1.0f);
      glm::vec2 size = glm::vec3(1.0f, 1.0f, 0.0f);
      glm::vec3 pos = glm::vec3(0.0f);
      Shader shader;
      Image tex_type;
      Texture texture;
      Vertex vertex;
      Data data;
   public:
      glm::mat4 get_projection(float zoom);
      void add_shaders(std::string src_vertex, std::string src_fragment);
      void add_texture(std::string src_texture, Image img_type);
    
      void set_data(Data d) { data = d; }
      void update() { model = glm::mat4(1.0f); }
      
      static void scale_object(glm::mat4 *model, glm::vec2 scaler);
      static void translate_object(glm::mat4 *model, glm::vec2 pos);
      static void rotate_object(glm::mat4 *model, float angle, glm::vec3 pos);
      void set_pos(glm::vec3 pos) { pos = pos; }

      void scale_object(glm::vec2 scaler){
         model = glm::scale(model, glm::vec3(scaler.x, scaler.y, 0.2f));
         this->size = scaler;
      }
      void translate_object(glm::vec2 pos){
         model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
         this->pos = {pos.x, pos.y, 0.0f};
      }
      void rotate_object(float angle, glm::vec3 pos){
         model = glm::rotate(model, angle, pos);
      }

      void draw(glm::mat4 &model, glm::mat4 view);
      void draw(glm::mat4 &model, glm::mat4 view, Texture texture);
      void draw(glm::mat4 &model, glm::mat4 view, glm::vec3 color);
};


class User: public Object {
   public: 
      int HP = 100, EXP = 0;
      glm::vec2 size = {0.2, 0.2};
      glm::vec2 pos = {1.0, 1.0};
   public:
      User(std::string src_vertex, std::string src_fragment, std::string src_texture, Image img_type, Data data):
      Object(src_vertex, src_fragment, src_texture, img_type, data){};
};

class Black_hole: public Object {
   public:
      Black_hole(): Object("../shaders/user.vert", "../shaders/user.frag", "../textures/black_hole.png", PNG){}
   public:
      void collide(Camera *camera, glm::vec2 pos_to, glm::vec2 size);

};


#endif 
