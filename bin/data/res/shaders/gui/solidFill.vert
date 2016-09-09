#version 150


// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

// this is the end of the default functionality

// this is something we're creating for this shader
out vec2 varyingTexcoord;
out vec4 vertexPosition;


void main()
{
    // here we move the texture coordinates
    varyingTexcoord = texcoord ;

    vertexPosition = modelViewProjectionMatrix * position;

    gl_Position = vertexPosition;
}
