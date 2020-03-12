precision mediump float;
varying vec3 v_norm;
varying vec2 v_uv;

varying vec3 fragPos;

varying vec4 fragPosLight[4];

uniform sampler2D texture1;

uniform sampler2D lightDepth1;
uniform sampler2D lightDepth2;
uniform sampler2D lightDepth3;
uniform sampler2D lightDepth4;

uniform samplerCube lightDepthCube1;
uniform samplerCube lightDepthCube2;
uniform samplerCube lightDepthCube3;
uniform samplerCube lightDepthCube4;


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

	vec3 projCoords1 = fragPosLight[0].xyz / fragPosLight[0].w * 0.5 + 0.5;
	float closestDepth1 = 0.0;
	if(lightType1.z == 1.0) closestDepth1 = texture2D(lightDepth1, projCoords1.xy).r;
	///if(lightType1.y == 1.0) closestDepth1 = textureCube(lightDepthCube1, lightDirRayVersor1).r;
	
	float shadow1 = //lightType1.y * float(closestDepth1 >= distance1 - 0.000005);
				    lightType1.z * float(closestDepth1 >= projCoords1.z - 0.000005);

    finalColorLight += length(lightType1) * 
	shadow1 *
	apply_light(lightType1, lightPos1, lightDir1, lightColor1, specularPow1, alpha1,
    normalVersor, viewDirectionVersor, vec3(objColor));

	vec3 projCoords2 = fragPosLight[1].xyz / fragPosLight[1].w * 0.5 + 0.5;
	float depth2 =  texture2D(lightDepth2, projCoords2.xy).r;
    finalColorLight += length(lightType2) * float(depth2 >= projCoords2.z-0.000005) * apply_light(lightType2, lightPos2, lightDir2, lightColor2, specularPow2, alpha2,
    normalVersor, viewDirectionVersor, vec3(objColor));

	vec3 projCoords3 = fragPosLight[2].xyz / fragPosLight[2].w * 0.5 + 0.5;
	float depth3 =  texture2D(lightDepth3, projCoords3.xy).r;
    finalColorLight += length(lightType3) * float(depth3 >= projCoords3.z-0.000005) * apply_light(lightType3, lightPos3, lightDir3, lightColor3, specularPow3, alpha3,
    normalVersor, viewDirectionVersor, vec3(objColor));

	vec3 projCoords4 = fragPosLight[3].xyz / fragPosLight[3].w * 0.5 + 0.5;
	float depth4 =  texture2D(lightDepth4, projCoords4.xy).r;
    finalColorLight += length(lightType4) * float(depth4 >= projCoords4.z-0.000005) * apply_light(lightType4, lightPos4, lightDir4, lightColor4, specularPow4, alpha4,
    normalVersor, viewDirectionVersor, vec3(objColor));

	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	vec3 finalColorFog = alpha * fogColor + (1.0-alpha) * finalColorLight;
    gl_FragColor = vec4(finalColorFog, objColor.a);

	/*vec2 coordE = ScreenPos.xy/ScreenPos.w*0.5+0.5;
	float depth = texture2D(lightDepth1, projCoords1.xy).x;
	float depth = projCoords1.z;
	float NEAR = 0.2;
	float FAR = 3000.0;
	float z = depth * 2.0 - 1.0; // Back to NDC 
    float c = (2.0 * NEAR) / (FAR + NEAR - z * (FAR - NEAR));
	gl_FragColor = vec4( projCoords1, 1.0);*/

} 
