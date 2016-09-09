// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect   tex0;

uniform int             nMax;
uniform int             nMin;

uniform vec3            oMax;
uniform vec3            oMin;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    vec3 texel = texture(tex0, varyingtexcoord).xyz;

    vec3 maxV = vec3((oMax.r/255.0),(oMax.g/255.0),(oMax.b/255.0));
    vec3 minV = vec3((oMin.r/255.0),(oMin.g/255.0),(oMin.b/255.0));

    vec3 lengths = vec3( maxV.r - minV.r , maxV.g - minV.g , maxV.b - minV.b );

    vec3 diff = vec3( (lengths.r - (texel.r - minV.r))/lengths.r , (lengths.g - (texel.g - minV.g))/lengths.g , (lengths.b - (texel.b - minV.b))/lengths.b);
    vec2 nValue = vec2(float(nMax)/255.0,float(nMin)/255.0);

    outputColor = vec4( (nValue.x - nValue.y) * diff.r + nValue.y,(nValue.x - nValue.y) * diff.g + nValue.y,(nValue.x - nValue.y) * diff.b + nValue.y , 1.0) ;
    //outputColor = vec4( texel, 1.0) ;
}
