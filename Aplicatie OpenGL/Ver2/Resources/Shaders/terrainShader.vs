attribute vec3 a_position;
attribute vec2 a_uv;
attribute vec2 a_uvBlend;



varying vec2 v_uv;
varying vec2 v_uvBlend;
varying vec3 fragPos;
varying vec4 textureColorBlend;


uniform sampler2D textureBlend;

uniform mat4 transfMatrix;
uniform vec2 moveTexture;
uniform vec3 heightMap;
uniform mat4 modelMatrix;


void main()
{
v_uv = a_uv;
v_uvBlend = moveTexture+a_uvBlend;
textureColorBlend = texture2D(textureBlend, v_uvBlend);

vec4 position = vec4(a_position, 1.0);
position.y += textureColorBlend.r * heightMap.r + textureColorBlend.g * heightMap.g + textureColorBlend.b * heightMap.b;

gl_Position = transfMatrix * position;
fragPos = vec3(modelMatrix * position);

}
   