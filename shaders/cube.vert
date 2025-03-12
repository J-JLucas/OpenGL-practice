#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main()
{
  // multiply homogenous vector by MVP matrix to convert to screen space
  gl_Position = uMVP * vec4(aPos, 1.0); 
}
