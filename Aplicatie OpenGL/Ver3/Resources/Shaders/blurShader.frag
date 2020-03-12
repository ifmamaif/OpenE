precision mediump float;
varying vec2 v_uv;

uniform sampler2D texture1;
uniform float r;

void main()
{
	vec4 obj_color1 = texture2D(texture1, v_uv);
	vec4 obj_color2 = texture2D(texture1, v_uv+vec2(r,0.0));
	vec4 obj_color3 = texture2D(texture1, v_uv-vec2(r,0.0));
	vec4 obj_color4 = texture2D(texture1, v_uv+vec2(0.0,r));
	vec4 obj_color5 = texture2D(texture1, v_uv-vec2(0.0,r));	
    gl_FragColor = (1.0*obj_color1 + obj_color2 + obj_color3 + obj_color4 + obj_color5)/5.0;
}
