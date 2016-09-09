// fragment shader
#version 150

// this is how we receive the texture
uniform vec3            lightColor;
uniform float           lightIntencity;
uniform vec3            eyePositionWorld;

in vec4                 fragmentPosition;
in vec3                 fragmentNormal;

out vec4                outputColor;


void main()
{
    float theDot = clamp(dot(normalize(eyePositionWorld-fragmentPosition.xyz), fragmentNormal),0.0,1.0);

    vec3 light = lightColor * lightIntencity ;// pow( , 10.25)

    float glow = clamp(pow(theDot,0.25),0.0,1.0);
    glow       = ( glow + max(pow(theDot,0.85), 0.0) );

    outputColor  = vec4(vec3(light * vec3(1.0) * glow ),1.0);
}
