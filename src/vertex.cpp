#include "game.h"

Vertex::Vertex(){
   glGenVertexArrays(1, &VAO);
}
Vertex::~Vertex(){
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
}

void Vertex::create_VBO(const void* data, size_t size){
   bind();
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
void Vertex::create_EBO(const void* data, size_t size){
   bind();
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

/* with EBO, size is lenght of indices buffer*/
void Vertex::draw(GLenum mode, size_t size){
   bind();
   glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
   unbind();
}
/* without EBO, just buffer, size is count of elements */
void Vertex::draw_buffer(GLenum mode, size_t size){
   bind();
   glDrawArrays(mode, 0, size);
   unbind();
}

void Vertex::add_attribute
(uint id, GLint size, GLenum type, 
GLboolean normalized, GLsizei stride, size_t offset)
{
   bind();
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glVertexAttribPointer(id, size, type, normalized, stride, (void*)offset);
   glEnableVertexAttribArray(id);
   unbind();

}

