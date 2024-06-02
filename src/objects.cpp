#include "collision.h"
#include "game.h"
#include "state.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <string>

void Object::add_shaders(std::string src_vertex, std::string src_fragment){
   shader.init_shader(src_vertex, src_fragment);
}

void Object::set_type(Image img_type){
   tex_type = img_type;
}

Object::Object(object_type type){
   float data_square[20] = {
        0.5f,  0.5f, 0.0f,  data.tex_tr.x, data.tex_tr.y,   // top right
        0.5f, -0.5f, 0.0f,  data.tex_br.x, data.tex_br.y,   // bottom right,
       -0.5f, -0.5f, 0.0f,  data.tex_bl.x, data.tex_bl.y,   // bottom left
       -0.5f,  0.5f, 0.0f,  data.tex_tl.x, data.tex_tl.y   // top left
   };

   if (type == circle){
      vertex.create_VBO(data_square, sizeof(data_square));
      vertex.create_EBO(indices_square, sizeof(indices_square));
      vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
      vertex.add_attribute(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3);
   } else {
      vertex.create_VBO(data_square, sizeof(data_square));
      vertex.create_EBO(indices_square, sizeof(indices_square));
      vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
   }

}
Object::Object(std::string src_vertex, std::string src_fragment, Image img_type): 
   shader(src_vertex, src_fragment), tex_type(img_type)
{
   if (img_type == NONE){
      vertex.create_VBO(data_dot, sizeof(data_dot));
      vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
      vertex.add_attribute(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3);
   } else if (img_type == LINES){
      vertex.create_VBO(square_vertices, sizeof(square_vertices));
      vertex.create_EBO(indices_square, sizeof(indices_square));
      vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
   }
}

Object::Object(std::string src_vertex, std::string src_fragment, Texture *texture_sheet, Texture_sheet coord):
   shader(src_vertex, src_fragment), tex_sheet(texture_sheet) 
{

   data = {
      .tex_tr{(coord.x_index*coord.sprite_width)/coord.sheet_width, (coord.y_index*coord.sprite_height)/coord.sheet_height},
      .tex_br{((coord.x_index+1)*coord.sprite_width)/coord.sheet_width, (coord.y_index*coord.sprite_height)/coord.sheet_height},
      .tex_bl{((coord.x_index+1)*coord.sprite_width)/coord.sheet_width, ((coord.y_index+1)*coord.sprite_height)/coord.sheet_height},
      .tex_tl{(coord.x_index*coord.sprite_width)/coord.sheet_width, ((coord.y_index+1)*coord.sprite_height)/coord.sheet_height},
   };

   float data_obj[] = {
        0.5f,  0.5f, 0.0f,  data.tex_tr.x, data.tex_tr.y,   // top right
        0.5f, -0.5f, 0.0f,  data.tex_br.x, data.tex_br.y,   // bottom right,
       -0.5f, -0.5f, 0.0f,  data.tex_bl.x, data.tex_bl.y,   // bottom left
       -0.5f,  0.5f, 0.0f,  data.tex_tl.x, data.tex_tl.y   // top left
   };

   vertex.create_VBO(data_obj, sizeof(data_obj));
   vertex.create_EBO(indices_square, sizeof(indices_square));
   vertex.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
   vertex.add_attribute(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3);
}


Object::~Object(){
   shader.delete_shader();
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

void Object::draw(glm::mat4 &model, glm::mat4 view, Texture texture){
   glm::mat4 proj = get_projection(global_states.zoom);
   if (tex_type != NONE && tex_type != LINES)
      tex_sheet->use();
   shader.use();
   shader.set_matrix4fv("proj", proj);
   shader.set_matrix4fv("view", view);
   shader.set_matrix4fv("model", model);
   if (tex_type == NONE)
      vertex.draw_buffer(GL_POINTS, 1);
   else if (tex_type == LINES)
      vertex.draw(GL_TRIANGLES, LEN(indices_square));
   else
      vertex.draw(GL_TRIANGLES, LEN(indices_square));
}
void Object::draw(glm::mat4 &model, glm::mat4 view){
   glm::mat4 proj = get_projection(global_states.zoom);
   if (tex_type != NONE && tex_type != LINES)
      tex_sheet->use();
   shader.use();
   shader.set_matrix4fv("proj", proj);
   shader.set_matrix4fv("view", view);
   shader.set_matrix4fv("model", model);
   if (tex_type == NONE)
      vertex.draw_buffer(GL_POINTS, 1);
   else if (tex_type == LINES)
      vertex.draw(GL_TRIANGLES, LEN(indices_square));
   else
      vertex.draw(GL_TRIANGLES, LEN(indices_square));
}

void Object::draw(glm::mat4 &model, glm::mat4 view, glm::vec3 color){
   glm::mat4 proj = get_projection(global_states.zoom);
   if (tex_type != NONE && tex_type != LINES)
      tex_sheet->use();
   shader.use();
   shader.set_matrix4fv("proj", proj);
   shader.set_matrix4fv("view", view);
   shader.set_matrix4fv("model", model);
   shader.set_vec3("color", color);
   if (tex_type == NONE)
      vertex.draw_buffer(GL_POINTS, 1);
   else if (tex_type == LINES)
      vertex.draw(GL_TRIANGLES, LEN(indices_square));
   else
      vertex.draw(GL_TRIANGLES, LEN(indices_square));
}

glm::mat4 Object::get_projection(float zoom){
   float aspect = (float)global_states.w_size.width/global_states.w_size.height;
   glm::mat4 proj = glm::ortho(-aspect*zoom, aspect*zoom, -1.0f*zoom, 1.0f*zoom, -1.0f, 1.0f);
   return proj;
}


void Black_hole::collide(Camera *camera, glm::vec2 pos_to, glm::vec2 size){
   if (!AABB_collision({pos+camera->pos, size, size.x/sqr_2}, {camera->pos, {2.0f, 2.0f}, 1.0f})){
      camera->pos = {pos_to, 0.0f};
    }
}

