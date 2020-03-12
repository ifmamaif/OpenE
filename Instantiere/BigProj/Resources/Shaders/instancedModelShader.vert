// Shader-ul de varfuri  
 
 #version 440


layout (location = 0)in vec3 a_position;
layout (location = 1)in vec3 a_color;
layout (location = 2)in vec3 a_norm;
//layout (location = 3) in vec2 a_uv; 

out vec4 gl_Position; 
out vec3 Color;
out vec3 Normal;
out vec3 FragPos;
//out vec2 uv;
 
in mat4 transfMatrixInstance;
in mat4 modelMatrixInstance;
 


void main(void)
  {
    gl_Position = transfMatrixInstance*vec4(a_position, 1.0);
	Color = a_color;
	//uv = a_uv;
    Normal=mat3(transpose(inverse(modelMatrixInstance))) * a_norm; 
	FragPos = vec3(modelMatrixInstance* vec4(a_position, 1.0f));
   } 
 