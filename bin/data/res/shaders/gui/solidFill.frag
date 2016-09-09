// fragment shader
#version 150

uniform vec4            gradientLimits;
uniform int             gradientType;

uniform float           tintModifier;
uniform vec4            tintColor;



in vec2                 varyingTexcoord;

out vec4                outputColor;

void main()
{
    float gradiant = 1.0;
    switch(gradientType)
    {
    case 0:
        gradiant = (gradientLimits.x + gradientLimits.y * sin(varyingTexcoord.x/1.0 * 3.14) ) +
                   (gradientLimits.z + gradientLimits.w * sin(varyingTexcoord.y/1.0 * 3.14) );
        break;
    case 1:
        gradiant = (gradientLimits.x + gradientLimits.y * varyingTexcoord.x/1.0) +
                   (gradientLimits.z + gradientLimits.w * varyingTexcoord.y/1.0);
        break;
    case 2:
        gradiant = 1.0;
        break;
    default:
        gradiant = 1.0;
        break;
    }

    float alpha = tintColor.a;

    outputColor  = vec4(vec3(tintColor.rgb * tintModifier) * min(gradiant,1.0) , alpha) ;
}
