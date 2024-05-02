#include "game.h"
#include "utils.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& src_vertex, const std::string& src_fragment){
   uint vertex, fragment;
   create_shader(&vertex, src_vertex, GL_VERTEX_SHADER); 
   create_shader(&fragment, src_fragment, GL_FRAGMENT_SHADER); 
   shader_id = glCreateProgram();
   glAttachShader(shader_id, vertex);
   glAttachShader(shader_id, fragment);
   glLinkProgram(shader_id);
   int res;
   char info[512];
   glGetProgramiv(shader_id, GL_LINK_STATUS, &res);
   if (!res){
      glGetProgramInfoLog(shader_id, 512, NULL, info);
      utils::error("shader", info);
   } else utils::log("shader is linked", std::to_string(shader_id));
   glDeleteShader(vertex);
   glDeleteShader(fragment);

}
void Shader::create_shader(uint *shader, const std::string& src, GLuint type){
   int res; char info[512];
   *shader = glCreateShader(type);
   std::string t = load_from_file(src);
   const char* code = t.c_str();
   glShaderSource(*shader, 1, &code, NULL);
   glCompileShader(*shader);
   glGetShaderiv(*shader, GL_COMPILE_STATUS, &res);
   if (!res){
      glGetShaderInfoLog(*shader, 512, NULL, info);
      utils::error(src, info);
   } else utils::log(src, "load shader");
}

void Shader::delete_shader(){
   glDeleteProgram(shader_id);
   utils::log("deleted shader", std::to_string(shader_id));
}

const int Shader::get_location(const std::string& name){
   int loc;
   if (cached_locations[name]) return cached_locations[name];
   loc = glGetUniformLocation(shader_id, name.c_str());
   if (loc == -1){
      utils::error("location", name);
      exit(-1);
   }
   cached_locations[name] = loc;
   return loc;
}

void Shader::set_float(const std::string& name, float x){
   uint loc = get_location(name);
   glUniform1f(loc, x);
}
void Shader::set_int(const std::string& name, int x){
   uint loc = get_location(name);
   glUniform1i(loc, x);
}
void Shader::set_vec3(const std::string& name, glm::vec3 x){
   uint loc = get_location(name);
   glUniform3f(loc, x.x, x.y, x.z);
}
void Shader::set_vec4(const std::string& name, glm::vec4 x){
   uint loc = get_location(name);
   glUniform4f(loc, x.x, x.y, x.z, x.w);
}
void Shader::set_matrix4fv(const std::string& name, glm::mat4 x){
   uint loc = get_location(name);
   glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(x));
}

 
