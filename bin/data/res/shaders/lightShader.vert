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
in vec2 texcoord;


// this is something we're creating for this shader
out vec4    fragmentPosition;
out vec3    fragmentNormal;


void main()
{
    fragmentPosition = translationMatrix * position;

    gl_Position = modelViewProjectionMatrix * fragmentPosition;

    // here we move the texture coordinates
    fragmentNormal = normalize( translationMatrix * vec4(normal, 0.0) ).xyz;
}
