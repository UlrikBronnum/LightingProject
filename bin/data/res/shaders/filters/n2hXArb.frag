// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect   tex0;

uniform int             texSize;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{


    vec3 texelBase = texture(tex0, varyingtexcoord).xyz;
    vec3 texelStep = texture(tex0, varyingtexcoord - vec2(1,0)).xyz;

    outputColor = vec4( vec3( (texelStep - (texelBase.x/texelBase.z) ) ), 1.0) ;
}

