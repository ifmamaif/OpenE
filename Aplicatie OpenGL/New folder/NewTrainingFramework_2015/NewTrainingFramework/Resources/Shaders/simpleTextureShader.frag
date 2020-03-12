precision mediump float;
varying vec2 v_uv;

uniform sampler2D texture1;

void main()
{
	vec4 obj_color = texture2D(texture1, v_uv);
	gl_FragColor = obj_color;
}
