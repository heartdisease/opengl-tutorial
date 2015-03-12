/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

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
