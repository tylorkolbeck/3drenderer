#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 tex;

uniform sampler2D sampler;

void main()
{
    FragColor = texture(sampler, tex);
}
