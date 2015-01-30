#ifndef __GL_BUFFER_OBJECT_H
#define __GL_BUFFER_OBJECT_H

#include <GL/glew.h>

#include "geometry.h"

typedef struct gl_buffer_object {
    GLuint id;
    GLenum type;
    geometry* geom;
} gl_buffer_object;

void create_array_buffer(gl_buffer_object* array_buffer);

void upload_buffer_data(gl_buffer_object* array_buffer, geometry* geom);

void draw_array_buffer(gl_buffer_object* array_buffer, GLuint attrib_array_index);

void delete_buffer_object(gl_buffer_object* buffer_object);

#endif
