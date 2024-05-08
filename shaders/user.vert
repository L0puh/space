#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 out_color;
out vec2 out_tex;

uniform mat4 model;
uniform mat4 proj;

void main()
{
   gl_Position = proj * model * vec4(pos, 1.0f);
   out_color = vec4(pos, 0.5f);
   out_tex = tex;
}

