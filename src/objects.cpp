#include "game.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <string>

Object::Object(std::string src_vertex, std::string src_fragment): shader(src_vertex, src_fragment), texture("", NONE), tex_type(NONE){
   vertex.create_VBO(data_dot, sizeof(data_dot));
   vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
   vertex.add_attribute(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3);
}

void Object::update(glm::vec3 pos, glm::vec2 scaler, float rotation){
   update();
   translate_object(pos);
   rotate_object(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
   scale_object({scaler.x, scaler.y});
}

void Object::update(glm::vec3 pos, glm::vec2 scaler){
   update();
   translate_object(pos);
   scale_object(scaler);
}

void Object::update(glm::vec3 pos, float rotation){
   update();
   translate_object(pos);
   rotate_object(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}

Object::Object(std::string src_vertex, std::string src_fragment, std::string src_texture, Image img_type):
   shader(src_vertex, src_fragment), texture(src_texture, img_type), tex_type(img_type)
{
   texture.load_texture();
   vertex.create_VBO(data_square, sizeof(data_square));
   vertex.create_EBO(indices_square, sizeof(indices_square));
   vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
   vertex.add_attribute(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3);
}

Object::~Object(){
   shader.delete_shader();
   texture.delete_texture();
}
void Object::scale_object(glm::vec2 scaler){
   model = glm::scale(model, glm::vec3(scaler.x, scaler.y, 0.0f));
   size = scaler;
}

void Object::translate_object(glm::vec2 pos_to){
   model = glm::translate(model, glm::vec3(pos_to.x, pos_to.y, 0.0f));
   pos = {pos_to.x - 1.0f, pos_to.y-1.0f, 0.0f};
   
}
void Object::rotate_object(float angle, glm::vec3 pos){
   model = glm::rotate(model, angle, pos);

}
void Object::scale_object(glm::mat4 *model, glm::vec2 scaler){
   *model = glm::scale(*model, glm::vec3(scaler.x, scaler.y, 0.0f));
}
void Object::translate_object(glm::mat4 *model, glm::vec2 pos){
   *model = glm::translate(*model, glm::vec3(pos.x, pos.y, 0.0f));
   
}
void Object::rotate_object(glm::mat4 *model, float angle, glm::vec3 pos){
   *model = glm::rotate(*model, angle, pos);
}

void Object::draw(GLFWwindow* window, glm::mat4 &model, glm::mat4 view){
   int width = get_window_size(window).width, 
       height = get_window_size(window).height;
   float aspect = (float)width/height;
   glm::mat4 proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f );
   if (tex_type != NONE)
      texture.use();
   shader.use();
   shader.set_matrix4fv("proj", proj);
   shader.set_matrix4fv("view", view);
   shader.set_matrix4fv("model", model);
   if (tex_type == NONE)
      vertex.draw_buffer(GL_POINTS, 1);
   else if (tex_type == LINES)
      vertex.draw_buffer(GL_LINES, 6);
   else
      vertex.draw(GL_TRIANGLES, LEN(indices_square));
}
/****************************************************/
/*                   USER                          */


/****************************************************/
/*                   PLANET                        */

