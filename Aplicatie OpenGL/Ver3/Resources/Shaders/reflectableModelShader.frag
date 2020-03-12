precision mediump float;

varying vec2 v_uv;
varying vec3 v_norm;

varying vec3 fragPos;

uniform samplerCube texture1;
uniform sampler2D texture2;

uniform float reflectiveness;
uniform float r;
uniform float R;
uniform vec3 fogColor;
uniform vec3 cameraPos;
void main()
{
vec4 objColor = texture2D(texture2, v_uv);
if (objColor.a<0.1)discard;


vec3 norm = normalize(v_norm);
vec3 viewDirVec = fragPos - cameraPos;
vec3 viewDirVersor = normalize(viewDirVec);
vec3 reflectDir = reflect(viewDirVersor, norm);
reflectDir = normalize(reflectDir);
 	
    
    vec4 mirrorColor = reflectiveness * textureCube(texture1, -reflectDir) + (1.0-reflectiveness) * objColor;	
	
	float d = sqrt(dot(viewDirVec, viewDirVec));
	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	vec3 finalColorFog = alpha * fogColor + (1.0-alpha) * vec3(mirrorColor);
    gl_FragColor = vec4(finalColorFog, objColor.a);
}
