// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform float           texSize;

uniform int             Hkernel[9];
uniform int             Vkernel[9];

uniform vec3            greyConvertion;
uniform float           scale;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    float valueH = 0.0;
    float valueV = 0.0;
    int diameter = 3;
    int radius = (diameter-1)/2;

    float stepSize = 1.0 / texSize;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++){
            vec4 color = texture(tex0, varyingtexcoord + vec2(float(x), float(y)) * stepSize);

            float grey = dot(color.rgb,greyConvertion);

            valueH += grey * float(Hkernel[(y+1) * diameter + (x+1)]);
            valueV += grey * float(Vkernel[(y+1) * diameter + (x+1)]);
        }
    }

    valueH = scale * valueH;
    valueV = scale * valueV;


    float length = sqrt( pow(valueH,2.0) + pow(valueV,2.0) + 1.0 );
    valueH = (-valueH / length) + 0.5;
    valueV = (-valueV / length) + 0.5;

    vec3 height = texture(tex0, varyingtexcoord).rgb;

    outputColor = vec4(valueV, valueH, 1.0 / length + 0.5 , 1.0 );//dot(height,greyConvertion)
    //outputColor = vec4(1.0);
}
