#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;
uniform vec4 borderColor;

void main()
{    
    FragColor = borderColor; 
}

