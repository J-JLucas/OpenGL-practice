#version 330 core
out vec4 FragColor;

// fragment shaders are all about calculating the output colour (r,g,b,a) of our pixels
void main()
{
  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);  // hardcoded to orange
}
