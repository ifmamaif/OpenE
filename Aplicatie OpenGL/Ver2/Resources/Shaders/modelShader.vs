attribute vec3 a_position;
attribute vec3 a_color;
attribute vec2 a_uv;


varying vec3 v_color;
varying vec2 v_uv;
varying vec3 fragPos;


uniform mat4 transfMatrix;
uniform mat4 modelMatrix;

void main()
{
vec4 position = vec4(a_position, 1.0);
gl_Position = transfMatrix * position;
v_color = a_color; //Interpolare
v_uv = a_uv;
fragPos = vec3(modelMatrix * position);

}
   