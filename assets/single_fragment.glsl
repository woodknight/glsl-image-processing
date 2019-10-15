#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

// texture samlers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{   
    FragColor = texture(texture1, TexCoord);
}