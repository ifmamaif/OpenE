precision highp float;
varying vec2 v_uv;

uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform float r;
uniform float R;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * r) / (R + r - z * (R - r));
}


void main()
{             
    float depthValue = texture2D(texture1, v_uv).r;
    gl_FragColor = vec4(vec3(LinearizeDepth(depthValue)), 1.0); // perspective
    // FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}