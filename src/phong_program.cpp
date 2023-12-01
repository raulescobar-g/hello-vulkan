#include <GL/glew.h>
#include "phong_program.h"
#include <iostream>
#include <string>
#include "util.h"

auto new_phong_program(const std::string& vertex_src, const std::string& fragment_src) -> phong_program {
    auto vertex_shader_source = read_file(vertex_src);
    auto fragment_shader_source = read_file(fragment_src);

    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vertex_shader_source, NULL); // vertex_shader_source is a GLchar* containing glsl shader source code
    glCompileShader(vshader);

    GLint vertex_compiled;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &vertex_compiled);
    if (vertex_compiled != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vshader, 1024, &log_length, message);
        std::cout << "Vertex shader compilation error: " << message << std::endl;
    }

    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fragment_shader_source, NULL); // fragment_shader_source is a GLchar* containing glsl shader source code
    glCompileShader(fshader);

    GLint fragment_compiled;
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &fragment_compiled);
    if (fragment_compiled != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fshader, 1024, &log_length, message);
        std::cout << "Fragment shader compilation error: " << message << std::endl;
    }

    uint pid = glCreateProgram();

    glAttachShader(pid, vshader);
    glAttachShader(pid, fshader);
    glLinkProgram(pid);

    GLint program_linked;
    glGetProgramiv(pid, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(pid, 1024, &log_length, message);
        std::cout << "Program link error: " << message << std::endl;
    }

    std::cout << "Successfully compiled shaders." << std::endl;
    
    std::string P_str = "P";
    std::string MV_str = "MV";
    std::string iMV_str = "iMV";
    std::string ka_str = "ka";
    std::string kd_str = "kd";
    std::string ks_str = "ks";
    std::string s_str = "s";
    std::string lightPos_s = "lightPos";
    std::string lightColor_s = "lightColor";

    int P = glGetUniformLocation(pid, P_str.c_str());
    int MV = glGetUniformLocation(pid, MV_str.c_str());
    int iMV = glGetUniformLocation(pid, iMV_str.c_str());
    int kd = glGetUniformLocation(pid, kd_str.c_str());
    int ka = glGetUniformLocation(pid, ka_str.c_str());
    int ks = glGetUniformLocation(pid, ks_str.c_str());
    int s = glGetUniformLocation(pid, s_str.c_str());
    int lightPos = glGetUniformLocation(pid, lightPos_s.c_str());
    int lightColor = glGetUniformLocation(pid, lightColor_s.c_str());

    return phong_program { pid, P, MV, iMV, ka, kd, ks, s, lightPos, lightColor };
}

auto drop_phong_program(phong_program& p) -> void { 
    glDeleteProgram(p.pid); 
}

auto bind(const phong_program& p) -> void {
    glUseProgram(p.pid);
}
auto unbind(const phong_program& _) -> void {
    glUseProgram(0);
}

