#version 330 core

vect4 out main

layout (location = 0) in vec3 apos;

void main(){

	gl_position = vec3(aPos.x, aPos.y, aPos.z, 1.0);

}