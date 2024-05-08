#version 330 core
out vec4 color;

in vec4 out_color;
in vec2 out_tex;
uniform sampler2D texture_sample;

void main()
{
   color = texture(texture_sample, out_tex);
}
