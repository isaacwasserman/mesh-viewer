#version 400
in vec3 Normal;
out vec4 FragColor;
void main()
{
   FragColor = vec4(Normal.x, Normal.y, Normal.z, 1.0);
}
