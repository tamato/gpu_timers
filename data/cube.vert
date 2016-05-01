#version 430

layout(location = 0) in vec4 Position;
layout(location = 2) in vec3 Normal;
layout(location = 8) in vec2 TexCoord;

layout(location = 10) uniform mat4 Model;
layout(location = 14) uniform mat4 View;
layout(location = 18) uniform mat4 Projection;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out vec3 outEyeVector; // position - camera (going in the neg Z)
layout(location = 3) out vec3 outLightDir;
// try keeping it all in model space

void main() {
    gl_Position = Projection * View * Model * Position;

    outNormal = vec3(Model * View * vec4(Normal.xyz,0));
    outEyeVector = (View * Model * Position).xyz;

    vec3 lightPos = (vec4(12,0,0,1)).xyz;
    outLightDir = normalize(lightPos - outEyeVector);
    outTexCoord = TexCoord;
}
