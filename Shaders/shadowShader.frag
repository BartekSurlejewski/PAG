#version 400 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform Light lights[3];

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 directionalLightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
	    
	float shadow = 0.0;

	if(!(projCoords.z > 1.0))
	{
		vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
				shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
			}
		}
		shadow /= 10.0;
	}

    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(directionalLightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;   

	for(int i = 0; i < 3; i++)
    {
        // diffuse
        vec3 lightDirSpot = normalize(lights[i].Position - FragPos);
        float diff = max(dot(lightDirSpot, normal), 0.0);
        vec3 result = lights[i].Color * diff * color;      
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(FragPos - lights[i].Position);
        result *= 1.0 / (distance * distance);
        lighting += result;           
    } 

// check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(lighting, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(lighting, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    
    FragColor = vec4(lighting, 1.0);
}