// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform vec3            channel;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 texel = texture(tex0, varyingtexcoord).xyz;

    outputColor = vec4( abs(channel - texel) , 1.0) ;
}
