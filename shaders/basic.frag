#version 330 core
out vec4 FragColor;

// fragment shaders are all about calculating the output colour (r,g,b,a) of our pixels
void main()
{
  FragColor = vec4(0.98f, 0.012f, 0.31f, 1.0f);  // hardcoded to fuchsia
}
