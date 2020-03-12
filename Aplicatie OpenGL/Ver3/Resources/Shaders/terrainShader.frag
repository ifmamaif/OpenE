precision mediump float;
varying vec2 v_uv;

varying vec4 textureColorBlend;
varying vec3 fragPos;


uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;


uniform float r;
uniform float R;
uniform vec3 fogColor;
uniform vec3 cameraPos;

void main()
{
	vec4 textureColor1 = texture2D(texture1, v_uv);
	vec4 textureColor2 = texture2D(texture2, v_uv);
	vec4 textureColor3 = texture2D(texture3, v_uv);
    vec4 terrainColor = textureColorBlend.r * textureColor1 + textureColorBlend.g * textureColor2 + textureColorBlend.b * textureColor3; 
      if (terrainColor.a <0.01)discard;
	
	
	vec3 viewDirVec = fragPos - cameraPos;
	float d = sqrt(dot(viewDirVec, viewDirVec));
	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	vec3 finalColorFog = alpha * fogColor + (1.0-alpha) * vec3(terrainColor);
    gl_FragColor = vec4(finalColorFog, terrainColor.a);
}
