precision mediump float;

varying vec3 fragPos;
varying vec3 v_coord;

uniform samplerCube texture1;


uniform float r;
uniform float R;
uniform vec3 fogColor;
uniform vec3 cameraPos;

void main()
{
	
    gl_FragColor = textureCube(texture1, v_coord);
	
	
	vec3 viewDirVec = fragPos - cameraPos;
 //gl_FragColor = vec4(viewDirVec, 1.0);
	float d = sqrt(dot(viewDirVec, viewDirVec));
	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	//gl_FragColor = vec4(alpha * fogColor, 1.0) + (1.0-alpha) * gl_FragColor;
}
