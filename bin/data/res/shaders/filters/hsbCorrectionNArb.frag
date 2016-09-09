// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;

uniform float           modHue;
uniform float           modSaturation;
uniform float           modLuminance;

in vec2                 varyingtexcoord;
out vec4                outputColor;


float hueAlign (float value){
    if(value > 1.0)
    {
        value = value - 1.0;
    }
    else if(value < 0.0)
    {
        value = value + 1.0;
    }
    return value;
}

float unrapChannel (float channelValue, float temporary1, float temporary2)
{
    float result = 6.0 * channelValue;
    if(result < 1.0)
    {
        return temporary2 + (temporary1-temporary2) * 6.0 * channelValue;
    }
    result = 2.0 * channelValue;
    if(result < 1.0)
    {
        return temporary1;
    }
    result = 3.0 * channelValue;
    if(result < 2.0)
    {
        return temporary2 + (temporary1-temporary2) * (0.666 - channelValue) * 6.0;
    }
    return temporary2;
}

float limitHue (float value){
    if(value > 360.0)
    {
        value = value - 360.0;
    }
    else if(value < 0.0)
    {
        value = value + 360.0;
    }
    return value;
}

void main()
{
    vec3 texel = texture(tex0, varyingtexcoord).xyz;

    float minimum = min(min(texel.r,texel.g),texel.b);
    float maximum = max(max(texel.r,texel.g),texel.b);
    float delta = maximum - minimum;

    float luminance = (minimum+maximum)/2.0;

    float saturation;
    float hue;

    if(luminance < 0.5){
        saturation = delta/(maximum + minimum);
    }else{
        saturation = delta/(2.0 - maximum - minimum);
    }

    if(maximum == minimum)
    {
        hue = 0;
    }
    else
    {
        if(maximum == texel.r)
        {
            hue = (texel.g-texel.b)/delta;
        }
        else if(maximum == texel.g)
        {
            hue = 2.0 + (texel.b-texel.r)/delta;
        }
        else if(maximum == texel.b)
        {
            hue = 4.0 + (texel.r-texel.g)/delta;
        }
        hue *= 60.0;
    }


    hue = limitHue( hue + modHue);
    saturation = clamp(saturation + modSaturation, 0.0, 1.0);
    luminance = clamp(luminance + modLuminance, 0.0, 1.0);


    float tmp1;
    float tmp2;

    if(luminance < 0.5)
    {
        tmp1 = luminance * (1.0 + saturation);
    }
    else{
        tmp1 = (luminance + saturation - luminance * saturation);
    }

    tmp2 = 2 * luminance - tmp1;

    float normHue = hue/360.0;

    float tmpRed = hueAlign(normHue + 0.333);
    float tmpGreen = hueAlign(normHue);
    float tmpBlue = hueAlign(normHue - 0.333);

    tmpRed = unrapChannel (tmpRed, tmp1, tmp2);
    tmpGreen = unrapChannel (tmpGreen, tmp1, tmp2);
    tmpBlue = unrapChannel (tmpBlue, tmp1, tmp2);

    //outputColor = vec4( vec3(hue/360.0,saturation,luminance) , 1.0) ;
    outputColor = vec4( vec3(tmpRed,tmpGreen,tmpBlue) , 1.0) ;
}
