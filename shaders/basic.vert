#version 330 core
layout(location = 0) in vec3 aPos;

// normally we would have to transform the vertex coordinates into normalized device coordinates ourselves
// but in this toy example program we hardcoded the triangle vertices in normalized device coordinates already
void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);  // gl_Position is a pre-defined output variable that holds the position of a vertex
}
