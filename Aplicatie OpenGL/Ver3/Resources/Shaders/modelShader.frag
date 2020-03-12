precision mediump float;
varying vec3 v_norm;
varying vec2 v_uv;

varying vec3 fragPos;

uniform sampler2D texture1;


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
     1.0 + lightType.z * (pow(max(cos(beta / alpha * PI_2),0.0), .5) - 1.0)			
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
    finalColorLight += length(lightType1) * apply_light(lightType1, lightPos1, lightDir1, lightColor1, specularPow1, alpha1,
    normalVersor, viewDirectionVersor, vec3(objColor));
    finalColorLight += length(lightType2) * apply_light(lightType2, lightPos2, lightDir2, lightColor2, specularPow2, alpha2,
    normalVersor, viewDirectionVersor, vec3(objColor));
    finalColorLight += length(lightType3) * apply_light(lightType3, lightPos3, lightDir3, lightColor3, specularPow3, alpha3,
    normalVersor, viewDirectionVersor, vec3(objColor));
    finalColorLight += length(lightType4) * apply_light(lightType4, lightPos4, lightDir4, lightColor4, specularPow4, alpha4,
    normalVersor, viewDirectionVersor, vec3(objColor));

	float alpha = (1.0)/(R-r)*(clamp(d, r, R)-r);
	vec3 finalColorFog = alpha * fogColor + (1.0-alpha) * finalColorLight;
    gl_FragColor = vec4(finalColorFog, objColor.a);

	/*float near_plane = 0.5, far_plane = 3000.0;
	float z1 = gl_FragCoord.z * 2.0 - 1.0; // Back to NDC 
    float z2 = (2.0 * near_plane) / (far_plane + near_plane - z1 * (far_plane - near_plane));
	gl_FragColor = vec4(vec3(z2), 1.0);*/
}
