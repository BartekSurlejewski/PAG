#version 400 core	
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D diffuseTeture;
uniform sampler2D shadowMap;

uniform vec4 color;
uniform vec3 directionalLightPos;
uniform vec3 viewPos;

struct Material
{
	// we have these, but do not use them at the moment
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//

	float specFactor;

	sampler2D texture_diffuse;
	sampler2D texture_specular;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
};

struct PointLight
{
	vec3 posWorld;
	vec3 att;	// Const / Linear / Quadratic

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
};

struct SpotLight
{
	vec3 posWorld;
	vec3 direction;
	vec3 att;
	float cutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;


uniform vec3 g_vCameraPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	//perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = 0.0f; 
	if(currentDepth > closestDepth)
	{
		shadow = 1.0f;
	}

	return shadow;
};

vec3 CalcDirectionalLight(DirectionalLight light, vec3 N, vec3 V)
{
	vec3 L = normalize( -light.direction );

	float NdotL = max( dot(N, L), 0.0f );

	vec3 H = normalize( L + V );
	float specFactor = pow(max(dot(N, H), 0.0), material.specFactor);

	// Combined results
	vec3 finalAmbient = light.ambient * vec3( texture(material.texture_diffuse, TexCoords) );
	vec3 finalDiffuse = light.diffuse * material.diffuse * NdotL * vec3( texture(material.texture_diffuse, TexCoords) );
	vec3 finalSpecular = light.specular * specFactor;

	//calculate shadow
	float shadow = ShadowCalculation(FragPosLightSpace);

	return (finalAmbient + (1.0 - shadow) * (finalSpecular + finalDiffuse));
};

vec3 CalcPointLight(PointLight light, vec3 N, vec3 V, vec3 posWorld)
{
	vec3 L = ( light.posWorld - posWorld );	// vector from PositionW to light pos

	float dist = length(L);
	float att = 1.0 / (light.att.x + light.att.y * dist + light.att.z * dist * dist);

	// normalize L
	L /= dist;

	float NdotL = max( dot(N, L), 0.0f );

	vec3 H = normalize( L + V );
	float specFactor = pow(max(dot(N, H), 0.0), material.specFactor);

	// Combined results
	vec3 finalAmbient = light.ambient * vec3( texture(material.texture_diffuse, TexCoords) );
	vec3 finalDiffuse = light.diffuse * NdotL * vec3( texture(material.texture_diffuse, TexCoords) );
	vec3 finalSpecular = light.specular * specFactor;

	finalAmbient *= att;
	finalDiffuse *= att;
	finalSpecular *= att;

	return finalAmbient + finalSpecular + finalDiffuse;
	//return finalAmbient;
};

vec3 CalcSpotLight(SpotLight light, vec3 N, vec3 V, vec3 posWorld)
{
	vec3 L = light.posWorld - posWorld;
	
	float dist = length(L);
	
	// normalize L
	L /= dist;

	float theta = dot(L, normalize(-light.direction));

	if (theta > light.cutoff)
	{
		float NdotL = max( dot(N, L), 0.0f );
		vec3 H = normalize( L + V );
		float specFactor = pow(max(dot(N, H), 0.0), material.specFactor);

		// Combined results
		vec3 finalAmbient = light.ambient * vec3( texture(material.texture_diffuse, TexCoords) );
		vec3 finalDiffuse = light.diffuse * NdotL * vec3( texture(material.texture_diffuse, TexCoords) );
		vec3 finalSpecular = light.specular * specFactor;

		float att = 1.0 / (light.att.x + light.att.y * dist + light.att.z * dist * dist);
		finalAmbient *= att;
		finalDiffuse *= att;
		finalSpecular *= att;

		return finalAmbient + finalDiffuse + finalSpecular;
	}
	else
	{
		return vec3(0, 0, 0);
	}
};

void main()
{
	vec3 N = normalize( Normal );
	vec3 V = normalize(  g_vCameraPos - FragPos.xyz );
	vec3 PosW = FragPos.xyz;

	// Calculate directional light
	vec3 result = CalcDirectionalLight(dirLight, N, V);

	// Calculate point light
	//result += CalcPointLight(pointLight, N, V, PosW);

	// Calculate spot light
	//result += CalcSpotLight(spotLight, N, V, PosW);

	//result *= color;

	// output
    FragColor =  vec4(result, 1.0f) * color;
};