// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect   tex0;
uniform sampler2DRect   tex1;

uniform float           strength;
uniform vec3            greyConvertion;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 texelBase = texture(tex0, varyingtexcoord).xyz;
    vec3 texelInvert = texture(tex1, varyingtexcoord).xyz;

    outputColor = vec4( vec3(dot(texelBase / (texelInvert * strength),greyConvertion)) , 1.0) ;
}
