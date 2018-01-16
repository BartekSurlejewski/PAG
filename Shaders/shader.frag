#version 400 core	
out vec4 FragColor;

in vec2 TexCoords;
in vec4 PositionW;
in vec3 NormalW;

uniform sampler2D texture_diffuse1;
uniform vec4 color;


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

	return finalAmbient + finalSpecular + finalDiffuse;
}

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
}

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
}

void main()
{
	vec3 N = normalize( NormalW );
	vec3 V = normalize(  g_vCameraPos - PositionW.xyz );
	vec3 PosW = PositionW.xyz;



	// Calculate directional light
	vec3 result = CalcDirectionalLight(dirLight, N, V);

	// Calculate point light
	result += CalcPointLight(pointLight, N, V, PosW);

	// Calculate spot light
	result += CalcSpotLight(spotLight, N, V, PosW);

	//result *= color;

	// output
    FragColor =  vec4(result, 1.0f) * color;
}