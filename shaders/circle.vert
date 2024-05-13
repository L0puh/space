#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform vec3 color;

out vec4 out_color;
out vec2 out_tex;

void main()
{
   gl_Position = proj * view * model * vec4(pos, 1.0f);
   out_color = vec4(color, 1.0f);
   out_tex = tex;
}

