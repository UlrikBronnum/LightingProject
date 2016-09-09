#version 150


// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform mat4 translationMatrix;


in vec4 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;


// this is something we're creating for this shader
out vec4    fragmentPosition;

out vec3    fragmentNormal;
out mat3    tbnMat;
out vec3    fragmentTangent;

out vec2    varyingTexcoord;


void main()
{
    fragmentPosition = translationMatrix * position;

    gl_Position = modelViewProjectionMatrix * fragmentPosition;

    // here we move the texture coordinates
    varyingTexcoord = texcoord ;

    fragmentNormal  = normalize( (translationMatrix * vec4(normal,0.0)).xyz );

    vec3 n = fragmentNormal;
    vec3 t = normalize(translationMatrix * vec4(tangent,0.0)).xyz;
    t = normalize(t - dot(t,n) * n);
    vec3 bT = normalize(cross(t,n));

    fragmentTangent = t;

    tbnMat = mat3(t,bT,n);


}
