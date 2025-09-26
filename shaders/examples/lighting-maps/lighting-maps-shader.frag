#version 330 core

struct MaterialNonMap {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform MaterialNonMap materialPlane;
uniform bool mapped;

uniform Light light;

uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main()
{
    // Logic to differeniate objects using spec + diff maps for lighting
    float mapFlag = float(mapped);
    float emissiveStrength = 1.0f;

    vec3 texturedDiff = vec3(texture(material.diffuse, TexCoords));
    vec3 texturedSpec = vec3(texture(material.specular, TexCoords));
    vec3 texturedEmissive = vec3(texture(material.emissive, TexCoords).rgb);
    float texturedShininess = material.shininess;

    vec3 nonTexturedDiff = materialPlane.diffuse;
    vec3 nonTexturedSpec = materialPlane.specular;
    vec3 nonTexturedEmissive = vec3(0.0f, 0.0f, 0.0f);
    float nonTexturedShininess = materialPlane.shininess;

    // Using the mappped bool as 0% or 100% mix saves using an if statement!
    vec3 DIFF = mix(nonTexturedDiff, texturedDiff, mapFlag);
    vec3 SPEC = mix(nonTexturedSpec, texturedSpec, mapFlag);
    vec3 EMIS = mix(nonTexturedEmissive, texturedEmissive, mapFlag);
    float SHINE = mix(nonTexturedShininess, texturedShininess, mapFlag);

    // ambient
    vec3 ambient = light.ambient * DIFF;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // ensures we do not exceed 0 as the floor, dot ranges (-1.0 - 1.0)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * DIFF;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * SPEC;

    // Emissive
    vec3 emissive = EMIS * emissiveStrength;

    // Final
    vec3 result = ambient + diffuse + specular + emissive;
    FragColor = vec4(result, 1.0);
}