// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;

uniform int             texSize;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    float stepSize = 1.0 / float(texSize);

    vec3 texelBase =  texture(tex0,varyingtexcoord ).x;

    vec3 texelStepX = texture(tex0, varyingtexcoord - vec2(stepSize,0)).x;
    vec3 texelStepY = texture(tex0, varyingtexcoord - vec2(0,stepSize)).x;

    float modifier1 = (-1.0 * pow(texelInvert.r, 2.0) + texelInvert.r) * 4.0;

    float maxValue = min(texelStepX.x - texelBase.x * 0.5, texelStepY.y - texelBase.y * 0.5);
    outputColor = vec4( vec3(maxValue)  , 1.0) ;
}
