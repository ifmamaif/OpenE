precision mediump float;
varying vec2 v_uv;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	
	vec4 obj_color1 = texture2D(texture1, v_uv);
	vec4 obj_color2 = texture2D(texture2, v_uv);
	if(obj_color2.r!=0.0)
		gl_FragColor = obj_color1+obj_color2*0.5;
	else 
		gl_FragColor = obj_color1;
}
