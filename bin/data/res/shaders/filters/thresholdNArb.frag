// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;

uniform vec3            thresholdGreater;
uniform vec3            thresholdLesser;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 texel = texture(tex0, varyingtexcoord).xyz;

    if(texel.r >= thresholdGreater.r/255.0 && texel.r <= thresholdLesser.r/255.0)
    {
        texel.r = 1.0;
    }
    else
    {
        texel.r = 0.0;
    }


    if(texel.g >= thresholdGreater.g/255.0 && texel.g <= thresholdLesser.g/255.0)
    {
        texel.g = 1.0;
    }
    else
    {
        texel.g = 0.0;
    }

    if(texel.b >= thresholdGreater.b/255.0 && texel.b <= thresholdLesser.b/255.0)
    {
        texel.b = 1.0;
    }
    else
    {
        texel.b = 0.0;
    }

    outputColor = vec4( texel , 1.0) ;
}
