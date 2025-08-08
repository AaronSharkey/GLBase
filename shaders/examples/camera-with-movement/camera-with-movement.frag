#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

//texture sampler
uniform sampler2D containerTexture;
uniform sampler2D awesomefaceTexture;

void main()
{
    FragColor = mix(texture(containerTexture, TexCoord), texture(awesomefaceTexture, TexCoord), 0.25);
}