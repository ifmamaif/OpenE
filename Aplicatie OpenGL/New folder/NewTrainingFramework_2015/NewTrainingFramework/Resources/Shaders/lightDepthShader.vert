attribute vec3 a_position;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

void main()
{
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(a_position, 1.0);
}
   