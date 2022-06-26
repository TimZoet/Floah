#version 450

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform usampler2D font;

void main()
{
    float alpha = texture(font, inTexCoord).r / 255.0;
    outColor = vec4(1, 1, 1, alpha) * inColor;
}