#version 140
in vec3 vertexAttrib;
in vec3 colorAttrib; // lesson2

out vec3 outColor; // lesson2

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main() {
	outColor = colorAttrib; // lesson2
	gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexAttrib, 1.0);
}
