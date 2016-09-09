// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect   tex0;

uniform int             brightness;
uniform int             contrast;


in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 color = texture(tex0, varyingtexcoord).xyz;
    //Brightness
    vec3 texel = vec3(color.r + (brightness/255.0),color.g + (brightness/255.0),color.b + (brightness/255.0));
    //Contrast
    float factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
    texel = factor * (texel - 0.5) + 0.5;

    outputColor = vec4( texel , 1.0) ;
}
