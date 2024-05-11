#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "game.h"
#include "utils.h"

void Texture::load_texture(){
   if (type == NONE) return;
   stbi_set_flip_vertically_on_load(1);
   unsigned char* src_texture = stbi_load(filename.c_str(), &width, &height, &bit_depth, 0);
   if (!src_texture) utils::error("failed to find", filename);

   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
   switch(type){
      case PNG:
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, src_texture);
         break;
      case JPG:
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, src_texture);
         break;
   }
   glGenerateMipmap(GL_TEXTURE_2D);
   stbi_image_free(src_texture);
   utils::log("loaded texture", filename);
}

void Texture::delete_texture(){
   utils::log("deleted texture:", std::to_string(texture_id));
   glDeleteTextures(1, &texture_id);
}

void Texture::use(){
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unuse(){
   glBindTexture(GL_TEXTURE_2D, 0);
}

