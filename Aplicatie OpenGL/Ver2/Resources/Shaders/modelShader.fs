precision mediump float;
varying vec3 v_color;
varying vec2 v_uv;

varying vec3 fragPos;

uniform sampler2D texture1;


uniform float r;
uniform float R;
uniform vec3 fogColor;
uniform vec3 cameraPos;
void main()
{
    gl_FragColor = texture2D(texture1, v_uv); 
    if (gl_FragColor.a <0.1)discard;
	
	
	vec3 viewDirVec = fragPos - cameraPos;
	float d = sqrt(dot(viewDirVec, viewDirVec));
	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	gl_FragColor = vec4(alpha * fogColor, 1.0) + (1.0-alpha) * gl_FragColor;
}
