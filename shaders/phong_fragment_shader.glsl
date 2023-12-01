#version 410 core

uniform vec3 lightPos;
uniform vec3 lightColor;

in vec3 fragPos; 
in vec3 fragNorm;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

out vec4 fragColor; 

void main() {

    vec3 l = normalize(lightPos - fragPos);
    vec3 h = normalize(l - normalize(fragPos));
    vec3 n = normalize(fragNorm);

    vec3 diffuse = kd * max(0.0, dot(n,l));
    vec3 specular = ks * pow(max(0.0, dot(n,h)),s);

    vec3 color = lightColor * (diffuse + specular);
    fragColor = vec4(color, 1.0); 
}

