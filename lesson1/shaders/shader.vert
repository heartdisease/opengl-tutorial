#version 330
layout(location = 0) in vec3 vertex;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main() {
	gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex, 1.0);
	//gl_Position = vec4(vertex, 1.0) * projectionMatrix;
}
