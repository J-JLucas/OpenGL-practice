// OpenGL automatically interpolates the colour across the triangle
// if we wanted a solid colour, we could use the "flat" keyword
// eg: flat out vec3 color;
// note, fragment shader must also have the same keyword
// eg: flat in vec3 color;

// also variable out's must be the exact name as variable in's in
// stage catching them eg in fragment shader

#version 330 core
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_color;

out vec3 color;

void main()
{
  gl_Position = vec4(v_pos, 1.0);  // can pass vector variable directly in and it parses elements automatically
  color = v_color;  // pipe color along! 
}
