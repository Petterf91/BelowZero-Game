#version 440 core
layout(location = 0) out vec4 color;

in vec2 texCoords;
//uniform int depthMapTransformation;

uniform sampler2D theTexture;
uniform sampler2D equipedTexture;
uniform sampler2D inventoryTexture1;
uniform sampler2D inventoryTexture2;
uniform sampler2D inventoryTexture3;
uniform sampler2D inventoryTexture4;
uniform sampler2D inventoryTexture5;
uniform sampler2D textTexture;
uniform sampler2D SceneTexture;
uniform sampler2D emilFusk;
//uniform sampler2D shadowMap1;
//uniform sampler2D shadowMap2;
//uniform sampler2D shadowMap3;


uniform float hp;
uniform float cold;
uniform float water;
uniform float food;
uniform float fade;
uniform float winFade;
uniform float flareTimer;

void main()
{
	vec3 fuskTextur = texture(emilFusk, texCoords).rgb;

	if (texture(equipedTexture, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(equipedTexture, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	else if (texture(theTexture, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(theTexture, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	else if (texture(inventoryTexture1, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(inventoryTexture1, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	else if (texture(inventoryTexture2, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(inventoryTexture2, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	else if (texture(inventoryTexture3, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(inventoryTexture3, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	else if (texture(inventoryTexture4, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(inventoryTexture4, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	else if (texture(inventoryTexture5, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(inventoryTexture5, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	else if (texture(textTexture, texCoords).w >= 0.01)
	{
		color.xyz = texture(SceneTexture, texCoords).xyz * abs(texture(textTexture, texCoords).w - 1);
		color.w = texture(SceneTexture, texCoords).w;
	}
	/*else if (texture(shadowMap1, texCoords).w >= 0.01 && depthMapTransformation == 1)
	{
		float depth = texture(shadowMap1, texCoords).x;
		color = vec4(depth, depth, depth, 1.0);
	}
	else if (texture(shadowMap2, texCoords).w >= 0.01 && depthMapTransformation == 2)
	{
		float depth = texture(shadowMap2, texCoords).x;
		color = vec4(depth, depth, depth, 1.0);
	}
	else if (texture(shadowMap3, texCoords).w >= 0.01 && depthMapTransformation == 3)
	{
		float depth = texture(shadowMap3, texCoords).x;
		color = vec4(depth, depth, depth, 1.0);
	}*/
	else
	{
		color = texture(SceneTexture, texCoords);
	}
	if (texCoords.x > 0.0433 && texCoords.x < 0.04 + (0.1585 * (hp / 100.0)) && texCoords.y > 0.028 && texCoords.y < 0.039)
		color.xyz = fuskTextur;//color.xyz = vec3(abs((hp / 100.0) - 1), hp / 100.0, 0.0);
	else if (texCoords.x > 0.0433 && texCoords.x < 0.04 + (0.1585 * (food / 100.0)) && texCoords.y > 0.074 && texCoords.y < 0.085)
		color.xyz = fuskTextur;//color.xzy = vec3(food / 100.0, 0.0, 0.0);
	else if (texCoords.x > 0.0433 && texCoords.x < 0.04 + (0.1585 * (water / 100.0)) && texCoords.y > 0.12 && texCoords.y < 0.132)
		color.xyz = fuskTextur;//color.xyz = vec3(0.0, 0.0, water / 100.0);
	else if (texCoords.x > 0.0433 && texCoords.x < 0.04 + (0.1585 * (cold / 100.0)) && texCoords.y > 0.167 && texCoords.y < 0.178)
		color.xyz = fuskTextur;//color.xyz = vec3(cold / 100, cold / 100, cold / 100);


	if (texture(theTexture, texCoords).w >= 0.01)
		color.xyz += texture(theTexture, texCoords).xyz;
	
	if (texture(equipedTexture, texCoords).w >= 0.01)
		color.xyz += texture(equipedTexture, texCoords).xyz;

	if (texture(inventoryTexture1, texCoords).w >= 0.01)
		color.xyz += texture(inventoryTexture1, texCoords).xyz;

	if (texture(inventoryTexture2, texCoords).w >= 0.01)
		color.xyz += texture(inventoryTexture2, texCoords).xyz;

	if (texture(inventoryTexture3, texCoords).w >= 0.01)
		color.xyz += texture(inventoryTexture3, texCoords).xyz;

	if (texture(inventoryTexture4, texCoords).w >= 0.01)
		color.xyz += texture(inventoryTexture4, texCoords).xyz;

	if (texture(inventoryTexture5, texCoords).w >= 0.01)
		color.xyz += texture(inventoryTexture5, texCoords).xyz;

	if (texture(textTexture, texCoords).w >= 0.01)
		color.xyz += texture(textTexture, texCoords).xyz;

	

	if (fade < 1 && fade > 0)
	{
		if (texture(textTexture, texCoords).w < 0.01)
			color.xyz = color.xyz + fade;
	}

	else if (fade >= 1 && texture(textTexture, texCoords).w < 0.01)
		color.xyz = vec3(1.0, 1.0, 1.0);



	if (flareTimer > 0)
	{
		color.xyz = color.xyz + winFade;
	}

}