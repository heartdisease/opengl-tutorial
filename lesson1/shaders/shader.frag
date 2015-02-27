#version 140
out vec4 fragColor;

in vec3 outColor; // lesson2

void main() {
	fragColor = vec4(outColor, 1.0); // lesson2
}
