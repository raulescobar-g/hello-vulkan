#version 410 core

uniform mat4 MV;  // Model-View matrix
uniform mat4 P;   // Projection matrix
uniform mat4 iMV; // Inverse Model-View matrix

layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNorm;   // Vertex normal

out vec3 fragPos;  // Position for fragment shader
out vec3 fragNorm; // Normal for fragment shader

void main() {
    fragPos = vec3(MV * vec4(aPos, 1.0));
    fragNorm = vec3(iMV * vec4(aNorm, 0.0));

    gl_Position = P * MV * vec4(aPos, 1.0);
}

