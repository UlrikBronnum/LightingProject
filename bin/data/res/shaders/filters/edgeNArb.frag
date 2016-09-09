// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform float           texSize;

uniform int             Hkernel[9];
uniform int             Vkernel[9];
uniform int             Dkernel[9];

uniform vec3            greyConvertion;
uniform int             outputSetting;
uniform int             thresholdValue;

in vec2                 varyingtexcoord;
out vec4                outputColor;

vec3 getThreshold(vec3 value, float threshold)
{
    float lThreshold = float(threshold)/255.0;
    value.r = (abs(value.r) > lThreshold) ? 1.0 : 0.0;
    value.g = (abs(value.g) > lThreshold) ? 1.0 : 0.0;
    value.b = (abs(value.b) > lThreshold) ? 1.0 : 0.0;
    return value;
}

vec3 getDifference(vec3 value)
{
    return vec3(0.5) + value;
}

vec3 getMagnitude(vec3 value)
{
    float length = sqrt( pow(value.r,2.0) + pow(value.g,2.0) + pow(value.b,2.0) );
    return vec3(length);
}

vec3 getNormalized(vec3 value)
{
    value /= 0.5;
    float length = sqrt( pow(value.r,2.0) + pow(value.g,2.0) + pow(value.b,2.0) );
    value /= length;
    return value;
}
vec3 getAbsolute(vec3 value)
{
    vec3 absolute = vec3( abs(value.r) + abs(value.g) + abs(value.b) )/3; // + vec3(0.5) ;max(value.r, max(value.g, value.b) ) );//
    return absolute;
}

void main()
{
    vec3 values = vec3(0.0);

    int diameter = 3;
    int radius = (diameter-1)/2;

    float stepSize = 1.0 / texSize;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++){
            vec4 color = texture(tex0, varyingtexcoord + vec2(float(x), float(y)) * stepSize);

            float grey = dot(color.rgb,greyConvertion);

            values.r += grey * float(Hkernel[(y+1) * diameter + (x+1)]);
            values.g += grey * float(Vkernel[(y+1) * diameter + (x+1)]);
            values.b += grey * float(Dkernel[(y+1) * diameter + (x+1)]);
        }
    }
    vec3 c;
    switch(outputSetting)
    {
    case 0:
        c = getThreshold(values, thresholdValue);
        break;
    case 1:
        c = getDifference(values);
        break;
    case 2:
        c = getMagnitude(values);
        break;
    case 3:
        c = getNormalized(values);
        break;
    case 4:
        c = getAbsolute(values);
        break;
    default:
        break;
    }
    outputColor = vec4(c,1.0);
}
