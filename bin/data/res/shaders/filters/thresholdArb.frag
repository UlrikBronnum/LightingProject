// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect   tex0;
uniform vec3            threshold;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 texel = texture(tex0, varyingtexcoord).xyz;

    texel.r = (texel.r > threshold.r/255.0) ? 1.0:0.0;
    texel.g = (texel.g > threshold.g/255.0) ? 1.0:0.0;
    texel.b = (texel.b > threshold.b/255.0) ? 1.0:0.0;


    outputColor = vec4( texel , 1.0) ;
}
