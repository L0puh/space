#version 330 core
out vec4 color;

in vec3 color_in;

void main()
{
   color = vec4(color_in, 1.0f);
}
