#version 440 core
layout(location = 0) out vec3 FragColor;

in vec2 TexCoords;
uniform vec3 view_position;

struct Light {
	vec3 Position;
	vec3 Color;
	int lightType;
	float intensity;
	float Linear;
	float Quadratic;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];

uniform float daylight;
uniform int nrOfLights;

uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
//uniform sampler2D shadowMap2;

uniform vec3 shadowMapLightPosition;
uniform float cascadeEndClipSpace[2];
uniform mat4 lightSpaceMatrix[2];
uniform mat4 viewMatrix;
uniform mat4 ProjectionMatrix;
uniform float water;

float clipSpacePosZ;
vec4 lightSpacePosition;



vec3 getBlur()
{
	const float offset = 1.0 / 50.0;

	vec2 blurOffsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
		);

	float blurKernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
		);

	vec3 blurSampleTex[9];
	vec3 blurColor = vec3(0.0);

	for (int i = 0; i < 9; i++)
		blurSampleTex[i] = vec3(texture(gAlbedo, TexCoords + blurOffsets[i]));

	for (int i = 0; i < 9; i++)
		blurColor += blurSampleTex[i] * blurKernel[i];

	return blurColor;
}


void calculateLightSpacePositions(vec4 FragPos, int index)
{
	lightSpacePosition = lightSpaceMatrix[index] * FragPos;
}

float cascadedShadowMapCalculation(int cascadeIndex, vec4 lightSpacePos, vec3 normals, vec3 FragPos)
{
	float shadow = 0.0f;
	float depth = 0.0f;
	float z;
	vec3 lightDirForShadow = normalize(shadowMapLightPosition - FragPos);
	float bias = max(0.05 * (1.0 - dot(normals, lightDirForShadow)), 0.005);
	vec3 projectionCoordinates = lightSpacePos.xyz;

	vec2 UVCoords;
	UVCoords.x = projectionCoordinates.x * 0.5 + 0.5;
	UVCoords.y = projectionCoordinates.y * 0.5 + 0.5;
	z = projectionCoordinates.z * 0.5 + 0.5;
	
	if (cascadeIndex == 0)							// Check Which ShadowMap To Use
		depth = texture(shadowMap0, UVCoords).x;
	if (cascadeIndex == 1)
		depth = texture(shadowMap1, UVCoords).x;
	//if (cascadeIndex == 2)
		//depth = texture(shadowMap2, UVCoords).x;

	if (z - bias > depth) // Determine If There Shall Be Shadow
		shadow = 0.1f;
	else
		shadow = 0.0f;


	return shadow;
}

void main()
{
	// retrieve data from G-buffer
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec4 FragPosition = texture(gPosition, TexCoords);
	vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	clipSpacePosZ = (ProjectionMatrix * viewMatrix * FragPosition).z;


	float Thirst = water;
	float thirstVariable = 1.0;
	if (Thirst <= 30)
	{
		thirstVariable = (Thirst - 20)/ 10;
	}
	if (Thirst <= 20)
		thirstVariable = 0;

	Albedo = mix(getBlur(), Albedo, thirstVariable);




	// then calculate lighting as usual
	vec3 lighting = vec3(0.0, 0.0, 0.0);
	vec3 viewDir = normalize(view_position - FragPos);



	for (int i = 0; i < nrOfLights; i++)
	{
		////dir
		//if (lights[i].lightType == 0) {
		//	// diffuse
		//	vec3 lightDir = normalize(lights[i].Position - FragPos);
		//	vec3 diffuse = max(dot(Normal, lightDir), 0.3) * Albedo * lights[i].Color;
		//	//diffuse = Albedo;
		//
		//	vec3 halfwayDir = normalize(lightDir + viewDir);
		//	float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
		//
		//	// attenuation
		//	float distance = length(lights[i].Position - FragPos);
		//	float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
		//	lighting += diffuse;
		//}

		//point
		if (lights[i].lightType == 1) {
			// diffuse
			vec3 lightDir = normalize(lights[i].Position - FragPos);
			float diffuse = max(dot(Normal, lightDir), 0.0);
			// attenuation
			float distance = length(lights[i].Position - FragPos);
			float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * (distance * distance));
			//float attenuation = 1.0 / ((distance * distance) / (lights[i].Linear * lights[i].Linear));

			vec3 diff = lights[i].Color * diffuse * Albedo;

			diff *= attenuation;

			lighting += diff * lights[i].intensity;
		}
	}

	//Directional Light
	// diffuse
	vec3 lightDir = normalize(vec3(7, 9, -5) - vec3(0.0, 0.0, 0.0));
	vec3 diffuse = max(dot(Normal, lightDir), 0.5) * Albedo * vec3(1, 1, 1);
	lighting += (diffuse * daylight);


	float density = 0.02;
	float gradient = 3.0;
	float distanceToPos = length(view_position - FragPos);
	float visibility = exp(-pow((distanceToPos * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	
	// =========================== TESTING SHADOWS ==================================
	float shadowFactor = 0.0;

	for (int i = 0; i < 2; i++)
	{
		if (clipSpacePosZ <= cascadeEndClipSpace[i])// Check Which Cascade To Sample from
		{
			calculateLightSpacePositions(FragPosition, i);
			shadowFactor = cascadedShadowMapCalculation(i, lightSpacePosition, Normal, FragPos);
			break;
		}
	}

	FragColor = lighting * (1.0f - shadowFactor);;
	FragColor = mix(vec3(0.749, 0.843, 0.823) * daylight, FragColor, visibility);

	
}

