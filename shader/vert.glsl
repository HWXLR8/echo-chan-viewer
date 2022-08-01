#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform float ascii_offset;  // offset only used when rendering text

void main() {
  TexCoords = vertex.zw;
  TexCoords.x += ascii_offset * (1.0/128.0);
  gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
