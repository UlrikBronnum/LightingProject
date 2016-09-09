// fragment shader
#version 150

// this is how we receive the texture


uniform vec3            sceneAmbientLight = vec3(0.1);
uniform vec4            sceneLightPosition;
uniform vec3            sceneLightDiffuseColor;
uniform vec3            sceneLightSpecularColor;
uniform vec3            sceneLightAttenuation = vec3(1.0,0.0,0.0);
uniform float           sceneLightIntencity = 3.0f;

uniform vec3            eyePositionWorld;

uniform sampler2D       diffuseTexture;
uniform sampler2D       normalTexture;
uniform sampler2D       specularTexture;
uniform vec4            tiling = vec4(1,1,0,0);

in vec4                 fragmentPosition;
in vec3                 fragmentNormal;
in vec3                 fragmentTangent;
in vec2                 varyingTexcoord;
in mat3                 tbnMat;


out vec4                outputColor;

vec3 calculateLight(vec4 lightPosition, vec3 lightDiffuseColor, vec3 lightSpecularColor)
{
    vec3 texelColor         = texture(diffuseTexture,varyingTexcoord * tiling.xy + tiling.zw).rgb;
    float texelSpec         = texture(specularTexture,varyingTexcoord * tiling.xy + tiling.zw).r;
    vec3 texelNormal        = normalize(tbnMat * (255.0/128.0 * texture(normalTexture,varyingTexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);

    vec3 diffuseLight;
    vec3 specularLight = vec3(0.0,0.0,0.0);

    vec3    lightDir;
    float   lDotN;
    float   attenuation;

    if(lightPosition.w > 0.0)
    {
        vec3 fragToLightSource = lightPosition.xyz-fragmentPosition.xyz;

        lightDir        = normalize(fragToLightSource);

        float distance  = length(fragToLightSource)/100.0f;

        attenuation     = 1.0 / (sceneLightAttenuation.x
                           + sceneLightAttenuation.y * distance
                           + sceneLightAttenuation.z * distance * distance);


        lDotN           = clamp( dot( lightDir , texelNormal ), 0.0, 1.0);
    }
    else
    {
        lightDir        = normalize(lightPosition.xyz);
        attenuation     = 1.0;
        lDotN           = clamp( dot( lightDir , texelNormal ), 0.0, 1.0);
    }

    // Diffuse Calculation
    diffuseLight  = attenuation * lightDiffuseColor * lDotN * sceneLightIntencity;

    // Specular Calculation
    if(lDotN > 0.0)
    {
        vec3 reflectionVector   = normalize(reflect(-lightDir,texelNormal));
        vec3 eyeVector          = normalize(eyePositionWorld - fragmentPosition.xyz);
        float specularity       = clamp(dot(reflectionVector,eyeVector), 0.0, 1.0);

        specularLight           = attenuation * clamp( lightSpecularColor * pow(specularity,64), 0.0, 1.0);

    }

    return sceneAmbientLight + texelColor * diffuseLight + texelSpec * specularLight;// clamp( 1.0-dot( texelColor , vec3(0.21,0.72,0.07) ), 0.0, 1.0) *
}

void main()
{
    outputColor             = vec4(calculateLight(sceneLightPosition, sceneLightDiffuseColor, sceneLightSpecularColor),1.0);
}
