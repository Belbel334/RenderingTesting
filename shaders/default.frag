#version 330 core
out vec4 FragColor;

uniform vec3 colorL;
uniform vec3 colorR;

void main()
{
    // Determine which square we're drawing based on gl_PrimitiveID
    // First 2 triangles (primitives 0-1) are left square
    // Next 2 triangles (primitives 2-3) are right square
    if (gl_PrimitiveID < 2)
        FragColor = vec4(colorL, 1.0);
    else
        FragColor = vec4(colorR, 1.0);
}
