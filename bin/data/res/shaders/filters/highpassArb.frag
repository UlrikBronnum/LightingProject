// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect   tex0;


in vec2                 varyingtexcoord;

in float                kernel[3025];
in int                  kernelRadius;

out vec4                outputColor;

void main()
{
    vec3 texel = texture(tex0, varyingtexcoord).xyz;

    outputColor = vec4( 1.0) ;
}
