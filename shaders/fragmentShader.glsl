#version 330 core

in vec2 texCoord;

uniform sampler2D myTexture;

out vec4 FragColor;

void main() {
    // FragColor = vec4(0.4, 0.2, 0.7, 1.0);
    FragColor = texture(myTexture, texCoord);
}