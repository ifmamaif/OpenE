attribute vec3 a_position;
attribute vec2 a_uv;

varying vec2 v_uv;
varying vec3 fragPos;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

void main()
{
v_uv = a_uv;

vec4 position = vec4(a_position, 1.0);
fragPos = vec3(modelMatrix * position);
gl_Position = viewProjectionMatrix * vec4(fragPos, 1.0);
}
   