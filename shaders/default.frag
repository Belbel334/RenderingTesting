#version 330 core
out vec4 FragColor;

uniform vec3 colorL;

void main()
{
  if (gl_PrimitiveID < 2)
    FragColor = vec4(colorL, 1.0);
}
