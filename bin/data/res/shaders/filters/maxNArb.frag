// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D   tex0;
uniform float       texSize;

uniform int         diameter;
uniform float       kernel[3025];

in vec2 varyingtexcoord;
out vec4 outputColor;


void main()
{
    int size = diameter;

    vec4 color = vec4(0.0,0.0,0.0,1.0);
    int radius = (size-1)/2;

    float stepSize = 1.0 / texSize;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++){
			vec4 pixelColor = vec4(texture(tex0, varyingtexcoord + vec2(float(x), float(y)) * stepSize).rgb,1);

            if(kernel[(y+radius) * size + (x+radius)] > 0.1){
                if (pixelColor.r > color.r){
                    color.r = pixelColor.r;
                }
                if (pixelColor.g > color.g){
                    color.g = pixelColor.g;
                }
                if (pixelColor.b > color.b){
                    color.b = pixelColor.b;
                }
            }
        }
    }
    outputColor = color;
}
