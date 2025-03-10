#version 330 core

in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D diffuse;

void main()
{
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.75)); // Direction of the light
    vec3 normal = normalize(Normal); // Normal vector

    // Calculate the diffuse lighting intensity
    // float diff = max(dot(normal, lightDir), 0.0);
    float diff = abs(dot(normal, lightDir));


    // Apply the lighting to the texture color
    vec3 texColor = vec3(texture(diffuse, TexCoords));
    vec3 lighting = diff * texColor;

    FragColor = vec4(lighting, 1.0);
}