#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aVertColor;

out vec3 VertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    VertColor = aVertColor;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
