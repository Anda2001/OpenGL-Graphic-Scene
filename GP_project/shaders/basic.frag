#version 410 core

// fragCoords
in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;

out vec4 fColor;

// lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

// lighting++
uniform int haveDirLight;

// texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

// fog
uniform float fogDensity;

// color
vec3 ambient;
float ambientStrength = 0.2f;
float ambientPointStrength = 0.001f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float specularPointStrength = 0.001f;
float shininess = 32.0f;

float constant = 1.0f;
float linear = 0.22f;
float quadratic = 0.20f;

uniform mat4 view;

float computeFog() 
{
	float fragmentDistance = length(fPosEye);
	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2)); //exponential fog ecuation
	return clamp(fogFactor, 0.0f, 1.0f); 
}


vec3 computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f); //in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;

	return ambient + diffuse + specular;
}

void main() 
{
	vec3 initialLight;
	if (haveDirLight == 0) {
		initialLight = computeLightComponents();
	}
	if (haveDirLight == 1) {
		initialLight = computeLightComponents();
	}
	if (haveDirLight == 2) {
		initialLight = vec3(1.0f, 0.0f, 0.0f);
	}
	
	

	float fogFactor = computeFog();
	vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f); //orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;

	vec3 color = min((ambient + diffuse) + specular, 1.0f);
	vec4 colorVec = vec4(color * initialLight, 1.0f);
	fColor = mix(fogColor, colorVec, fogFactor);
}
