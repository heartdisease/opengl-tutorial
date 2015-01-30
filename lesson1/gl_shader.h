#ifndef __GL_SHADER_H
#define __GL_SHADER_H

#include <GL/glew.h>

typedef struct gl_shader {
    GLuint id;
    char* source;
} gl_shader;

void create_vertex_shader(gl_shader* shader, GLchar* source_file);

void create_fragment_shader(gl_shader* shader, GLchar* source_file);

void compile_shader(gl_shader* shader);

void delete_shader(gl_shader* shader);

#endif
