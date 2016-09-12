// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform sampler2D       tex1;
uniform float           texSize;

uniform float           strength = 1.0;
uniform vec3            greyConvertion;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 texelBase = texture(tex0, varyingtexcoord).xyz;
    vec3 texelInvert = texture(tex1, varyingtexcoord).xyz;

    float baseValue = dot( texelBase,greyConvertion );
    float modValue =  dot( texelInvert,greyConvertion );

    float heightValue1 = baseValue / ((1.0 - modValue) * 1.05);//
    float heightValue2 = modValue / ((1.0 - baseValue) * 1.88);///pow( strength, 2.2 )

    int radius = 1;
    float stepSize = 1.0 / texSize;

    float neighborhoodHeight = 0.0;
    float neighborhoodMax = 0.0;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++)
        {
			float baseHeight = dot(texture(tex0, varyingtexcoord + vec2(float(x), float(y)) * stepSize).rgb,greyConvertion );
			float modHeight  = dot(texture(tex1, varyingtexcoord + vec2(float(x), float(y)) * stepSize).rgb,greyConvertion );

            float baseHeightValue = baseHeight / ((1.0 - modHeight) * 1.05);//
            float modHeightValue =  modHeight / ((1.0 - baseHeight) * 1.88);

            float value = clamp((baseHeight*(modHeightValue/baseHeightValue))*modHeight,0.0,1.0);
            neighborhoodHeight += value;

            if(value > neighborhoodMax)
                neighborhoodMax = value;

        }
    }

    neighborhoodHeight /= (radius*2+1)*(radius*2+1);
/*
    float modifier1 = (-1.2 * pow(texelInvert.r, 2.0) + 1.0 * texelInvert.r) * 2;
    float modifier2 = (-1.2 * pow(texelInvert.g, 2.0) + 1.0 * texelInvert.g) * 2;
    float modifier3 = (-1.2 * pow(texelInvert.b, 2.0) + 1.0 * texelInvert.b) * 2;
    float modifier4 = (-1.0 * pow(heightValue, 2.0) + 1.0 * heightValue) * 4;

    float intensity = min((modifier2*modifier1*modifier3),1.0);

    float blend     = (heightValue - modifier4 * pow( strength, 2.2 ))/(1.0-intensity) ; //0.65 / (modifier1 * 0.33 + modifier2 * 0.33 + modifier3 * 0.33) * pow( strength, 2.2 )
*/
    //outputColor = vec4( vec3(baseValue*(heightValue2/heightValue1)*modValue+neighborhoodHeight) , 1.0);
    //outputColor = vec4( vec3( pixelValue ) , 1.0);
    outputColor = vec4( texelBase / (1.0 - texelInvert) , 1.0);
}
