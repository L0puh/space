#version 330 core
layout (location = 0) in vec3 pos;

out vec4 in_color;

uniform mat4 model_un;
uniform mat4 view_un;
uniform mat4 proj_un;
uniform vec3 color_un;

void main()
{
   gl_Position = proj_un * view_un * model_un * vec4(pos.x, pos.y, pos.z, 1.0f);
   in_color = vec4(color_un, 1.0f);
}

