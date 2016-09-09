// fragment shader
#version 150

uniform vec2            texCoordMax;
uniform vec4            gradientLimits;
uniform int             gradientType;

uniform float           tintModifier;
uniform vec4            tintColor;
uniform sampler2D       texture;

in vec2                 varyingTexcoord;

out vec4                outputColor;

void main()
{
    vec4 textureColor = texture(texture,varyingTexcoord);

    float gradiant = 1.0;
    switch(gradientType)
    {
    case 0:
        gradiant = (gradientLimits.x + gradientLimits.y * sin(varyingTexcoord.x/texCoordMax.x * 3.14) ) +
                   (gradientLimits.z + gradientLimits.w * sin(varyingTexcoord.y/texCoordMax.y * 3.14) );
        break;
    case 1:
        gradiant = (gradientLimits.x + gradientLimits.y * varyingTexcoord.x/texCoordMax.x) +
                   (gradientLimits.z + gradientLimits.w * varyingTexcoord.y/texCoordMax.y);
        break;
    case 2:
        gradiant = 1.0;
        break;
    default:
        gradiant = 1.0;
        break;
    }

    float alpha = textureColor.a * tintColor.a;

    outputColor  = vec4(vec3(textureColor.rgb * tintColor.rgb * tintModifier) * gradiant, alpha ) ;
}
