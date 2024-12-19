#version 330 core

layout (location = 0) in vec3 aPos;
out vec4 vertexColor;

void main() {
  gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
  vertexColor = vec4(0.0, 0.28, 0.56, 1.0);
}