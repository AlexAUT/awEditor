varying vec2 uv;

uniform sampler2D baseColor_tex;

void main()
{
  gl_FragColor = vec4(texture2D(baseColor_tex, uv).rgb, 1.0);
}
