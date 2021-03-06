// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;

uniform float           texSize;
uniform int             radius;


in vec2                 varyingtexcoord;

out vec4                outputColor;

void main()
{
    vec3 texelColor = texture(tex0, varyingtexcoord).rgb;

    vec3 color = vec3(0.0,0.0,0.0);

    float stepSize = 1.0 / texSize;

    int diameter = radius * 2 + 1;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++)
        {
			vec3 pixelColor = texture(tex0, varyingtexcoord + vec2(float(x), float(y)) * stepSize).rgb;
            if (y == 0 && x == 0) {
                color +=  pixelColor * (float(diameter*diameter-1)/float(diameter*diameter));
            }
            else {
                color +=  pixelColor * (-1.0f / float(diameter*diameter));
            }
        }
    }

    outputColor = vec4(color, 1.0) ;
}
