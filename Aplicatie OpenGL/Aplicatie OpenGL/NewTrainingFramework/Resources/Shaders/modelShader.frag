precision mediump float;
varying vec3 v_norm;
varying vec2 v_uv;

varying vec3 fragPos;

varying vec4 fragPosLight[2];

uniform sampler2D texture1;

uniform sampler2D lightDepth1[6];
uniform sampler2D lightDepth2[6];

uniform mat4 lightMatrix[12];

uniform float r;
uniform float R;
uniform vec3 fogColor;

uniform vec3 cameraPos;

uniform vec3 lightType1;
uniform vec3 lightType2;
uniform vec3 lightType3;
uniform vec3 lightType4;

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 lightPos4;

uniform vec3 lightDir1;
uniform vec3 lightDir2;
uniform vec3 lightDir3;
uniform vec3 lightDir4;

uniform vec3 lightColor1;
uniform vec3 lightColor2;
uniform vec3 lightColor3;
uniform vec3 lightColor4;

uniform float specularPow1;
uniform float specularPow2;
uniform float specularPow3;
uniform float specularPow4;

uniform float alpha1;
uniform float alpha2;
uniform float alpha3;
uniform float alpha4;

uniform vec3 ambiental_light_color;
uniform float ratio;
uniform float kDiffuse;
uniform float kSpecular;

const float PI_2 = 1.57079632679489661923;
const float near = 0.2;
const float far = 3000.0;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near) / (far + near - z * (far - near));
}
float ProjectiveDepth(float depth)
{
    float z = (((2.0 * near) / depth) - (far + near)) / (near - far);
    return (z + 1.0)*0.5;
}
vec3 apply_ambiental_light(vec3 ObjectColor)
{
 return ratio *  ambiental_light_color * ObjectColor;
}
vec3 apply_light(vec3 lightType, vec3 lightPos, vec3 lightDir, vec3 lightColor, float specularPow, float alpha, 
  vec3 normalVersor, vec3 viewDirectionVersor, vec3 ObjectColor)
  { 
  vec3 lightDirRay = lightType.x * lightDir + (1.0-lightType.x)*(fragPos - lightPos);
  vec3 lightDirVersor = normalize(lightDirRay);
  vec3 reflectDirVersor = reflect(lightDirVersor, normalVersor);
  float distance = (1.0 - lightType.x) * sqrt(dot(lightDirRay, lightDirRay));

  float diffuseFactor = max(dot(normalVersor, -lightDirVersor), 0.0);
  vec3 diffuseColor = ObjectColor * lightColor * diffuseFactor * kDiffuse;

  float specularFactor = pow(max(dot(reflectDirVersor, - viewDirectionVersor), 0.0), specularPow);
  vec3 specularColor = lightColor * specularFactor * kSpecular;

  vec3 finalColor = (diffuseColor + specularColor) / (1.0 + 0.001*distance);
  float cosbeta = lightType.z * dot(normalize(lightDir), lightDirVersor);
  float beta = acos(cosbeta);

  return (1.0-ratio) * finalColor * (
     1.0 + lightType.z * (pow(max(cos(beta / alpha * PI_2),0.0), 1.0) - 1.0)			
	);
}

void main()
{
    vec4 objColor = texture2D(texture1, v_uv); 
    if (objColor.a <0.1)discard;
	
	
	vec3 viewDirVec = fragPos - cameraPos;
	float d = sqrt(dot(viewDirVec, viewDirVec));
    
    vec3 viewDirectionVersor = normalize(viewDirVec);
    vec3 normalVersor = normalize(v_norm);


    vec3 finalColorLight = apply_ambiental_light(vec3(objColor));

	vec3 lightDirRay1 = fragPos - lightPos1;
	vec3 lightDirRayVersor1 = normalize(lightDirRay1);
	float distance1 = length(lightDirRay1);
	float max_value = max(abs(lightDirRay1.x), abs(lightDirRay1.y));
	max_value = max(max_value, abs(lightDirRay1.z));
	vec3 normalizedRayCubeMap = lightDirRay1 / max_value;
	vec3 projCoords1 = fragPosLight[0].xyz / fragPosLight[0].w * 0.5 + 0.5;
	vec3 Coordonate;
	vec4 fragLight;
	float closestDepth1 = 0.0;
	if(lightType1.y != 1.0) closestDepth1 = texture2D(lightDepth1[0], projCoords1.xy).r;
	if(lightType1.y == 1.0)
	{ 
	if(abs(normalizedRayCubeMap.x - 1.0)<0.001)
		{closestDepth1 = texture2D(lightDepth1[0], (normalizedRayCubeMap.zy)*0.5+0.5).r;
		fragLight = lightMatrix[0] * vec4(fragPos, 1.0);
		Coordonate = fragLight.xyz / fragLight.w * 0.5 + 0.5;
	}
	else if(abs(normalizedRayCubeMap.x + 1.0)<0.001)
		{closestDepth1 = texture2D(lightDepth1[1], (vec2(-normalizedRayCubeMap.z, normalizedRayCubeMap.y))*0.5+0.5).r;
	fragLight = lightMatrix[1] * vec4(fragPos, 1.0);
		Coordonate = fragLight.xyz / fragLight.w * 0.5 + 0.5;
	}
	else if(abs(normalizedRayCubeMap.y - 1.0)<0.001)
		{closestDepth1 = texture2D(lightDepth1[2], (vec2(-normalizedRayCubeMap.z, normalizedRayCubeMap.x))*0.5+0.5).r;
	fragLight = lightMatrix[2] * vec4(fragPos, 1.0);
		Coordonate = fragLight.xyz / fragLight.w * 0.5 + 0.5;
	}
	else if(abs(normalizedRayCubeMap.y + 1.0)<0.001)
		{closestDepth1 = texture2D(lightDepth1[3], (normalizedRayCubeMap.zx)*0.5+0.5).r;
	fragLight = lightMatrix[3] * vec4(fragPos, 1.0);
		Coordonate = fragLight.xyz / fragLight.w * 0.5 + 0.5;
	}
	else if(abs(normalizedRayCubeMap.z - 1.0)<0.001)
		{closestDepth1 = texture2D(lightDepth1[4], (vec2(-normalizedRayCubeMap.x, normalizedRayCubeMap.y))*0.5+0.5).r;
			fragLight = lightMatrix[4] * vec4(fragPos, 1.0);
		Coordonate = fragLight.xyz / fragLight.w * 0.5 + 0.5;
	}
	else if(abs(normalizedRayCubeMap.z + 1.0)<0.001)
		{closestDepth1 = texture2D(lightDepth1[5], (normalizedRayCubeMap.xy)*0.5+0.5).r;
	fragLight = lightMatrix[5] * vec4(fragPos, 1.0);
		Coordonate = fragLight.xyz / fragLight.w * 0.5 + 0.5;
	}
	}
	float bias = 0.0000*tan(acos(clamp(0.0,1.0,dot(normalVersor, lightDirRayVersor1))));
	//float bias = max(0.005 * (1.0 - clamp(0.0,1.0,dot(normalVersor, lightDirRayVersor1))), 0.0005); 
	float shadow1 = //lightType1.y * float(closestDepth1 >= Coordonate.z - 0.000005);
				    (1.0-lightType1.y) * float(closestDepth1 >= projCoords1.z - 0.0025);

    finalColorLight += length(lightType1) * 
	shadow1 *
	apply_light(lightType1, lightPos1, lightDir1, lightColor1, specularPow1, alpha1,
    normalVersor, viewDirectionVersor, vec3(objColor));

	vec3 projCoords2 = fragPosLight[1].xyz / fragPosLight[1].w * 0.5 + 0.5;
	float depth2 =  texture2D(lightDepth2[0], projCoords2.xy).r;
    finalColorLight += length(lightType2) * float(depth2 >= projCoords2.z-0.000005) * apply_light(lightType2, lightPos2, lightDir2, lightColor2, specularPow2, alpha2,
    normalVersor, viewDirectionVersor, vec3(objColor));

	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	vec3 finalColorFog = alpha * fogColor + (1.0-alpha) * finalColorLight;
    gl_FragColor = vec4(finalColorFog, objColor.a);

	
	//gl_FragColor = vec4(projCoords1, 1.0);
	//float s1 = LinearizeDepth(texture2D(lightDepth1[4], (-normalizedRayCubeMap.xy)*0.5+0.5).r);
	//float s2 = (distance1-near)/(far-near);
	//gl_FragColor = vec4(vec3(LinearizeDepth(Coordonate.z)), 1.0);
	//gl_FragColor = vec4(vec3(((distance1-near)/(far-near))), 1.0);
	//gl_FragColor = vec4(vec3(float(s1<s2+.3)),1.0);
	//gl_FragColor = vec4(vec3(float(abs(normalizedRayCubeMap.y - 1.0)==0.000)), 1.0);
} 
