#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirectionLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float attn_const;
    float attn_linear;
    float attn_quad;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutoff;    
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float attn_const;
    float attn_linear;
    float attn_quad;
};

uniform Material material;

uniform DirectionLight directionLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform Spotlight flashlight;
  
uniform vec3 viewPos;

in vec3 oNormal;
in vec3 oFragPos;
in vec2 TexCoords;

vec3 calcDirectionLight(DirectionLight aLight, vec3 aNormal, vec3 aViewDir)
{
    vec3 lightDir = normalize(-aLight.direction);
    
    // diffuse factor
    float diff = max(dot(aNormal, lightDir), 0.0); 

    //specular factor
    vec3 reflectDir = reflect(-lightDir, aNormal); 
    float spec = pow(max(dot(aViewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = aLight.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = aLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = aLight.specular * spec * texture(material.specular, TexCoords).rgb;
    return ambient + diffuse + specular; 
}

vec3 calcSpotLight(Spotlight aLight, vec3 aNormal, vec3 aViewDir)
{
    vec3 lightDir = normalize(-aLight.direction);
    
    // diffuse
    float diff = max(dot(aNormal, lightDir), 0.0);

    // specular
    vec3 reflectDir = reflect(-lightDir, aNormal);
    float spec = pow(max(dot(aViewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = aLight.ambient * texture(material.diffuse, TexCoords).rgb; 
    vec3 diffuse = aLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = aLight.specular * spec * texture(material.specular, TexCoords).rgb;

    // attenuation
    float dist = length(aLight.position - oFragPos);
    float attenuation = 1.0 / (aLight.attn_const + aLight.attn_linear * dist + aLight.attn_quad * (dist * dist));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;    

    // spotlight intensity
    float ctheta = dot(lightDir, normalize(-aLight.direction)); 
    float epsilon = aLight.cutoff - aLight.outerCutoff;
    float intensity = clamp((ctheta - aLight.outerCutoff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight aLight, vec3 aNormal, vec3 aFragPos, vec3 aViewDir)
{
    vec3 lightDir = normalize(aLight.position - oFragPos);

    // diffuse factor
    float diff = max(dot(aNormal, lightDir), 0.0);

    // specular factor
    vec3 reflectDir = reflect(-lightDir, aNormal);
    float spec = pow(max(dot(aViewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = aLight.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = aLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = aLight.specular * spec * texture(material.specular, TexCoords).rgb;

    // attenuation
    float dist = length(aLight.position - oFragPos);
    float attenuation = 1.0 / (aLight.attn_const + aLight.attn_linear * dist + aLight.attn_quad * (dist * dist));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 normal = normalize(oNormal);
    vec3 viewDir = normalize(viewPos - oFragPos);

    // direction light
    vec3 color = calcDirectionLight(directionLight, normal, viewDir); 

    // point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        color += calcPointLight(pointLights[i], normal, oFragPos, viewDir);
    }

    // spot light
    color += calcSpotLight(flashlight, normal, viewDir);

    FragColor = vec4(color, 1.0);
}