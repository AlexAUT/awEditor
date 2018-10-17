attribute vec3 vPosition;
attribute vec2 vTexCoord;

uniform mat4 mvp_matrix;

varying vec2 uv;

void main()
{
  gl_Position = mvp_matrix * vec4(vPosition, 1.0);

  uv = vTexCoord;
}
