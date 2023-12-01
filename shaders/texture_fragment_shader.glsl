#version 410 core

in vec3 fragPos; // Position from vertex shader
in vec3 fragNorm; // Normal from vertex shader
in vec2 fragTex;

uniform sampler2D texture0;

out vec4 fragColor; // Final color output

void main() {
    fragColor = texture(texture0, fragTex);
}

