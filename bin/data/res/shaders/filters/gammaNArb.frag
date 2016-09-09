// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;

uniform float           gamma;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 texel = texture(tex0, varyingtexcoord).xyz;
    float gammaCorrection = 1.0 / gamma;

    outputColor = vec4( pow(texel.r,gammaCorrection),pow(texel.g,gammaCorrection),pow(texel.b,gammaCorrection) , 1.0) ;
}
