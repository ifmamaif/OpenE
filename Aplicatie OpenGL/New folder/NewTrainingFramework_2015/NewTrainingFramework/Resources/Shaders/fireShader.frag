precision mediump float;
varying vec2 v_uv;

varying vec3 fragPos;


uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

uniform float u_Time;
uniform float u_DispMax;
uniform float r;
uniform float R;
uniform vec3 fogColor;
uniform vec3 cameraPos;

void main()
{
	vec4 fireMask = texture2D(texture2, v_uv);
    vec2 displacement = texture2D(texture3, vec2(v_uv.x, v_uv.y + u_Time)).rg;
    vec2 offset = u_DispMax * (2.0*displacement-1.0);
	vec4 fire = texture2D(texture1, v_uv + offset);
	fire.a *= fireMask.r;
	
	vec3 viewDirVec = fragPos - cameraPos;
	float d = sqrt(dot(viewDirVec, viewDirVec));
	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	vec3 finalColorFog = alpha * fogColor + (1.0-alpha) * vec3(fire);
    gl_FragColor = vec4(finalColorFog, fire.a);
}
