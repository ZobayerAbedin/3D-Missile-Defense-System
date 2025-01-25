#version 330 core

layout(location = 0) in vec3 aPos; // Vertex position
layout(location = 1) in vec3 aNormal; // Normal vector

out vec3 fragNormal; // Output normal to fragment shader
out vec3 fragPos; // Output position to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0)); // Compute the position in world space
    fragNormal = mat3(transpose(inverse(model))) * aNormal; // Compute the normal in world space
    gl_Position = projection * view * vec4(fragPos, 1.0);
}
