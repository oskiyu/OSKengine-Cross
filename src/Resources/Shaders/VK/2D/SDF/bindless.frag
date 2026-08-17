#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "../signed_distance_fields.glsl"

layout(location = 0) flat in int inShapeType;
layout(location = 1) flat in int inContentType;
layout(location = 2) in vec4 inMainColor;
layout(location = 3) in vec4 inSecondaryColor;
layout(location = 4) in vec2 inTexCoords;
layout(location = 5) flat in vec2 inCenterPosition;
layout(location = 6)  flat in float inBorderWidth;
layout(location = 7) flat in int inInstanceIndex;
layout(location = 8) flat in int inFill;

layout (set = 1, binding = 1) uniform sampler2D images[];

layout (set = 0, binding = 0) uniform GlobalInformation {
    vec2 resolution;
    mat4 cameraMatrix;
} globalInformation;

layout (location = 0) out vec4 outColor;

void main() {
    float sdfValue = -1.0;
    if (inShapeType == 0) {
        sdfValue = -1.0; // Rectangle.
    }
    else if (inShapeType == 1) {
        sdfValue = Sdf2D_Circle(gl_FragCoord.xy, inCenterPosition, 1);
    }
    sdfValue = clamp(-sdfValue, 0, 1);

    const bool fill = inFill == 1;
    if (!fill){
       sdfValue = Sdf2D_Hullify(sdfValue, inBorderWidth);
    }

    vec4 color = inMainColor;
    if (inContentType == 0) {
        color = inMainColor * texture(images[nonuniformEXT(inInstanceIndex)], inTexCoords); // Texture.
    }
    else if (inContentType == 1) {
        color = inMainColor; // Flat color.
    }
    
    if (inInstanceIndex == 0) {
        color = vec4(0.5, 0.5, 0.5, 1);
    }
    if (inInstanceIndex == 1) {
        color = vec4(1, 0, 0, 1);
    }
    if (inInstanceIndex == 2) {
        color = vec4(0, 1, 0, 1);
    }
    if (inInstanceIndex == 3) {
        color = vec4(0, 0, 1, 1);
    }
    if (inInstanceIndex == 4) {
        color = vec4(1, 1, 0, 1);
    }
    if (inInstanceIndex == 5) {
        color = vec4(1, 0, 1, 1);
    }
    
    outColor = color * sdfValue;
}
