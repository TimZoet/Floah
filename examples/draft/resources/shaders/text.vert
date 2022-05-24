#version 450

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoord;

void main()
{
    gl_Position = vec4(inPosition.x / 320 - 1, inPosition.y / 320 - 1.0, 0, 1);
    outColor = inColor;
    outTexCoord = inTexCoord;
}
