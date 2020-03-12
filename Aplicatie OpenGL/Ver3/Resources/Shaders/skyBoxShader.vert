attribute vec3 a_position;

varying vec3 fragPos;
varying vec3 v_coord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;


void main()
{

v_coord = a_position;
vec4 position = vec4(a_position, 1.0);

fragPos = vec3(modelMatrix * position);
gl_Position = viewProjectionMatrix * vec4(fragPos, 1.0);

}
   