#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 color;

out vec3 color_in;

void main()
{
   gl_Position = proj * view * model * vec4(pos.x, pos.y, pos.z, 1.0f);
   color_in = color;
}

