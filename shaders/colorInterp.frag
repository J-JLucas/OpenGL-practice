#version 330 core
in vec3 color;
out vec4 fragColor;

// fragment shaders are all about calculating the output colour (r,g,b,a) of our pixels
void main()
{
  fragColor = vec4(color, 1.0f); 
}
