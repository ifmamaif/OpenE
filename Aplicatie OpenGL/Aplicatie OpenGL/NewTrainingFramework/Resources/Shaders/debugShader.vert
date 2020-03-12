attribute vec3 a_position;
attribute vec3 a_color;
attribute vec3 a_norm;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

//varying vec3 v_color;
void main()
{
//v_color = a_color;
vec4 position = vec4(a_position, 1.0);
 gl_Position = viewProjectionMatrix * (modelMatrix * position +
    a_color.y*vec4(15.0*normalize(mat3(normalMatrix)*a_norm), 0.0));
}
   