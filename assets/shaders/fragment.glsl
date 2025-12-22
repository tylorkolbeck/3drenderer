#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 tex;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, tex), texture(texture2, tex), 0.2);
}
