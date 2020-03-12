attribute vec3 a_position;
attribute vec3 a_norm;
attribute vec2 a_uv;


varying vec3 v_norm;
varying vec2 v_uv;
varying vec3 fragPos;

varying vec4 fragPosLight[4];

uniform mat4 viewProjectionMatrix;
uniform mat4 lightMatrix[4];

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main()
{
v_uv = a_uv;

vec4 position = vec4(a_position, 1.0);

fragPos = vec3(modelMatrix * position);
v_norm = mat3(normalMatrix) * a_norm;
gl_Position = viewProjectionMatrix * vec4(fragPos, 1.0);
for(int i = 0; i < 4; i++)
	fragPosLight[i] = lightMatrix[i] * vec4(fragPos, 1.0);
}
   