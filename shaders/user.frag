#version 330 core
out vec4 color;

in vec2 in_tex;
in vec4 in_color;

uniform sampler2D texture_sample;

void main()
{
   color = texture(texture_sample, in_tex) * in_color;
}
