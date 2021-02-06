#version 450 core

in vec3 FragPosition;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPosition; // This is the light position in world space, we are going to assume its a white light for now
uniform vec3 cameraPosition; // camera position in world space

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

    // Diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * 0.5) * vec3(1.0, 1.0, 1.0);

    float specularStrength = 0.6;
    vec3 viewDir = normalize(cameraPosition - FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);  

    vec3 result = (ambient + diffuse + specular) * vec3(0.0, 1.0, 0.0);
	FragColor = vec4(result, 1.0);
}