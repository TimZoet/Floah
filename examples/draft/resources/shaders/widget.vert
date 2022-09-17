#version 450

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoord;

layout(set = 0, binding = 0) uniform WindowTransform
{
    vec4 lower;
    vec4 upper;
} windowTransform;

layout(push_constant) uniform WidgetTransform
{
    layout(offset = 0) vec4 panelOffset;
    layout(offset = 16) vec4 widgetOffset;
} widgetTransform;

void main()
{
    vec4 range = windowTransform.upper - windowTransform.lower;
    gl_Position = vec4(
        (inPosition.x + widgetTransform.panelOffset.x + widgetTransform.widgetOffset.x - windowTransform.lower.x) / range.x * 2.0 - 1.0,
        (inPosition.y + widgetTransform.panelOffset.y + widgetTransform.widgetOffset.y - windowTransform.lower.y) / range.y * 2.0 - 1.0,
        1.0 - (inPosition.z + widgetTransform.panelOffset.z + widgetTransform.widgetOffset.z - windowTransform.lower.z) / range.z,
        1
    );
    outColor = inColor;
    outTexCoord = inTexCoord;
}