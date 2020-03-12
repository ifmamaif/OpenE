precision mediump float;
varying vec2 v_uv;

uniform sampler2D texture1;


void main()
{
	vec4 obj_color = texture2D(texture1, v_uv);
	float grayscale = 0.2126 * obj_color.r + 0.7152 * obj_color.g + 0.1 * 0.0722;
	float brightness = dot(vec3(obj_color), vec3(0.2126, 0.7152, 0.0722));
	gl_FragColor = vec4(step(0.35, brightness) * vec3(grayscale,grayscale,grayscale), 1.0);
}
