#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 in_tex;
out vec4 in_color;

uniform mat4 model_un;
uniform mat4 proj_un;
uniform mat4 view_un;
uniform vec3 color_un;

void main()
{
   gl_Position = proj_un * view_un * model_un * vec4(pos, 1.0f);
   in_tex = tex;
   in_color = vec4(color_un, 1.0f);
}

