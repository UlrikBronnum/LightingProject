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

uniform int radius;
// this is something we're creating for this shader
out vec2    varyingtexcoord;
out float   kernel[500];


void main()
{

    int diameter = radius * 2 + 1;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++)
        {
            if (y == 0 && x == 0) {
                kernel[(y + radius) * diameter + (x + radius)] = float(diameter*diameter-1)/float(diameter*diameter);
            }
            else {
                kernel[(y + radius) * diameter + (x + radius)] = -1.0f / float(diameter*diameter);
            }
        }
    }
    //kernelRadius = radius;
    // here we move the texture coordinates
    varyingtexcoord = texcoord;
    // send the vertices to the fragment shader
    gl_Position = modelViewProjectionMatrix * position;
}
