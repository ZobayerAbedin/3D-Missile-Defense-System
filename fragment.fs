#version 330 core

in vec3 fragNormal; // Incoming normal vector
in vec3 fragPos; // Incoming position

out vec4 FragColor; // Final color output

uniform vec4 material.ambient;
uniform vec4 material.diffuse;
uniform vec4 material.specular;
uniform float material.shininess;

uniform vec3 lightPos; // Light position

void main()
{
    // Ambient
    vec4 ambient = material.ambient;

    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = material.diffuse * diff;

    // Specular
    vec3 viewDir = normalize(-fragPos); // View direction
    vec3 reflectDir = reflect(-lightDir, norm); // Reflection direction
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = material.specular * spec;

    // Combine results
    FragColor = ambient + diffuse + specular;
}
