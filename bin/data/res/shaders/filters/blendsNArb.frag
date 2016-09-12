// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform sampler2D       tex1;

uniform int             blendMode;


in vec2                 varyingtexcoord;
out vec4                outputColor;


vec3 colorDarken(vec3 target, vec3 blend)
{
    return vec3( min(target.r, blend.r), min(target.g, blend.g), min(target.b, blend.b) );
}

vec3 colorLighten(vec3 target, vec3 blend)
{
    return vec3( max(target.r, blend.r), max(target.g, blend.g), max(target.b, blend.b) );
}

vec3 colorMult(vec3 target, vec3 blend)
{
    return vec3(target * blend);
}

vec3 colorBurn(vec3 target, vec3 blend)
{
    return vec3(1.0) - vec3( (vec3(1.0) - target) / blend );
}

vec3 colorLinearBurn(vec3 target, vec3 blend)
{
    return target + blend - vec3(1.0);
}

vec3 colorScreen(vec3 target, vec3 blend)
{
    return vec3(1.0) - vec3( (vec3(1.0) - target) * (vec3(1.0) - blend) );
}

vec3 colorDodge(vec3 target, vec3 blend)
{
    return vec3(target / (vec3(1.0) - blend));
}

vec3 colorLinearDodge(vec3 target, vec3 blend)
{
    return target + blend;
}

vec3 colorDifference(vec3 target, vec3 blend)
{
    return vec3( abs(target.r - blend.r), abs(target.g - blend.g), abs(target.b - blend.b));
}

vec3 colorExclusion(vec3 target, vec3 blend)
{
    return vec3(0.5) - 2.0 * (target-vec3(0.5)) - (blend * vec3(0.5));
}

vec3 colorSubtract(vec3 target, vec3 blend)
{
    return vec3( max(target.r - blend.r,0.0), max(target.g - blend.g,0.0), max(target.b - blend.b,0.0));
}

void main()
{
    vec3 texelTarget = texture(tex0, varyingtexcoord).xyz;
    vec3 texelBlend = texture(tex1, varyingtexcoord).xyz;

    vec3 color;

    if(blendMode == 0)
    {
        color = colorDarken(texelTarget, texelBlend);
    }
    else if(blendMode == 1)
    {
        color = colorLighten(texelTarget, texelBlend);
    }
    else if(blendMode == 2)
    {
        color = colorMult(texelTarget, texelBlend);
    }
    else  if(blendMode == 3)
    {
        color = colorBurn(texelTarget, texelBlend);
    }
    else if(blendMode == 4)
    {
        color = colorLinearBurn(texelTarget, texelBlend);
    }
    else if(blendMode == 5)
    {
        color = colorScreen(texelTarget, texelBlend);
    }
    else  if(blendMode == 6)
    {
        color = colorDodge(texelTarget, texelBlend);
    }
    else if(blendMode == 7)
    {
        color = colorLinearDodge(texelTarget, texelBlend);
    }
    else if(blendMode == 8)
    {
        color = colorDifference(texelTarget, texelBlend);
    }
    else if(blendMode == 9)
    {
        color = colorExclusion(texelTarget, texelBlend);
    }
    else if(blendMode == 10)
    {
        color = colorSubtract(texelTarget, texelBlend);
    }
    outputColor = vec4( color , 1.0);
}
