#version 400 core	
out vec4 FragColor;

in vec2 TexCoords;
in vec4 FragPos;
in vec3 Normal;

uniform sampler2D diffuseTexture;

struct PointLight
{
	vec3 posWorld;
	vec3 att;	// Const / Linear / Quadratic

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
};

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;


uniform vec3 g_vCameraPos;

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

void main()
{
	vec3 N = normalize( Normal );
	vec3 V = normalize(  g_vCameraPos - FragPos.xyz );
	vec3 PosW = FragPos.xyz;

	// Calculate point light
	result += CalcPointLight(pointLight, N, V, PosW);

	// output
    FragColor =  vec4(result, 1.0f) * color;
}