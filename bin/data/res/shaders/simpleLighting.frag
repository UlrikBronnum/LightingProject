// fragment shader
#version 150

// this is how we receive the texture


uniform vec3            sceneAmbientLight        = vec3(0.2);
uniform vec4            sceneLightPosition;
uniform vec3            sceneLightColor;



in vec4                 fragmentPosition;
in vec3                 fragmentNormal;
in vec2                 varyingTexcoord;

out vec4                outputColor;


vec3 calculateDiffuseLight(vec4 lightPosition, vec3 lightColor, vec3 lightAttenConst)
{
    vec3    lightDir;
    float   attenuation;

    if(lightPosition.w == 1.0){
        vec3 fragToLightSource = normalize(lightPosition.xyz-fragmentPosition.xyz);

        float distance  = length(fragToLightSource);

        attenuation     = 1.0 / (lightAttenConst.x
                           + lightAttenConst.y * distance
                           + lightAttenConst.z * distance * distance);

        lightDir       = normalize(fragToLightSource);
    }
    else
    {
        attenuation     = 1.0;
        lightDir        = normalize(lightPosition.xyz);
    }

    vec3 diffuseLight  = attenuation * lightColor * clamp( dot( fragmentNormal, lightDir ), 0.0, 1.0);

    return diffuseLight;
}


void main()
{

    vec3 attenConst = vec3(0.0,1.0,0.0);

    vec3 diffuse    = vec3(1.0)  * calculateDiffuseLight(sceneLightPosition, sceneLightColor, attenConst);
    vec3 ambient    = vec3(1.0)  * sceneAmbientLight;

    outputColor  = vec4(ambient + diffuse ,1.0);
    //outputColor  = vec4(fragmentNormal ,1.0);
}
