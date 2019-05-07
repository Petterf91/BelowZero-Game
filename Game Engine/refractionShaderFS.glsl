#version 440 core
layout(location = 0) out vec3 FragColor;

in vec2 TexCoords;
in vec3 cornerPos;
uniform vec3 view_position;


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D iceNormal;
uniform sampler2D SceneTexture;
uniform sampler2D DamageTexture;

uniform mat4 vMatrix;

uniform float healthSeconds;
uniform float hp;
uniform float cold;
uniform float water;
uniform float food;
uniform float fade;

uniform int ScreenX;
uniform int ScreenY;

vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
	);



void main()
{
	float Cold = cold;
	float Hunger = food;
	float Thirst = water;
	float Health = hp;


	vec2 texCoords = vec2((gl_FragCoord.x / ScreenX), (gl_FragCoord.y / ScreenY));

	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Albedo = texture(SceneTexture, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;

	vec3 viewDir = normalize(view_position - FragPos);

	vec3 ice = texture(iceNormal, TexCoords).rgb;
	vec3 bump = normalize(ice) * 2.0 - 1.0;
	bump = ice;
	bump *= normalize(viewDir);


	float _Refraction = 1.3;
	float _Frensel = 2.0;
	float _Reflectance = 1.0;

	vec3 n = normalize(ice);
	vec3 v = viewDir;

	float fr = pow(1.0f - dot(v, n), _Frensel) * _Reflectance;
	vec3 reflectDir = reflect(-v, n);
	vec3 refractDir = refract(-v, n, _Refraction);

	vec3 reflectColor = texture(SceneTexture, reflectDir.xy).rgb;
	vec3 refractColor = texture(SceneTexture, refractDir.xy).rgb;

	vec3 finalColor = reflectColor * fr + refractColor;
	vec3 Mix2 = mix(finalColor, Albedo, 0.8);


	vec3 bumpTex = 2.0 * texture(iceNormal, texCoords).rgb - 1.0;
	vec2 newUV = vec2(texCoords.x + bumpTex.x, texCoords.y + bumpTex.y);

	vec3 posDir = view_position - FragPos;
	posDir = vec4(vMatrix * vec4(posDir, 1.0)).xyz;

	bump = vec4(vMatrix * vec4(-bump, 1.0)).xyz;;
	vec3 itsADir = cornerPos - FragPos;

	vec3 Refract = normalize(refract(itsADir, -ice, 1.35));
	vec3 Reflect = normalize(reflect(itsADir, -bumpTex));
	vec3 testAlbedo1 = texture(SceneTexture, Refract.xy).rgb;
	vec3 testAlbedo2 = texture(SceneTexture, Reflect.xy).rgb;

	vec3 testAlbedo = (testAlbedo2 * testAlbedo1);
	vec3 Mix = mix(testAlbedo, Albedo, 0.7);
	//Mix *= vec3(0.33, 0.9, 1.0);

	float coldVariable = 1.0;
	if (Cold <= 30)
	{
		coldVariable = (Cold -20) / 10;
	}
	if (Cold <= 20)
		coldVariable = 0;

	float hungerVariable = 1.0;
	if (Hunger <= 30)
	{
		hungerVariable = (Hunger - 20) / 10;
	}
	if (Hunger <= 20)
		hungerVariable = 0;
   //
	//if (gl_FragCoord.x > ScreenX * 0.8)
	//	coldTexture = mix(Mix2, Albedo, coldVariable);
	//else if (gl_FragCoord.y > ScreenY * 0.7)
	//	coldTexture = mix(Mix2, Albedo, coldVariable);
	//else if (gl_FragCoord.x < ScreenX * 0.2)
	//	coldTexture = mix(Mix2, Albedo, coldVariable);
	//else if (gl_FragCoord.y < ScreenY * 0.3)
	//	coldTexture = mix(Mix2, Albedo, coldVariable);
   //
	vec3 coldTexture = mix(Mix, Albedo, coldVariable);

	float grayScale = dot(coldTexture, vec3(0.299, 0.587, 0.114));
	vec3 grayScaleTexture = vec3(grayScale, grayScale, grayScale);

	grayScaleTexture = mix(grayScaleTexture, coldTexture, hungerVariable);


	float healthVariable = 1.0;
	if (Health <= 30)
	{
		healthVariable = (Health) / 30;


	}
	
	vec4 healthColor = texture(DamageTexture, texCoords).rgba;

	vec3 mixHealth = mix(healthColor.xyz, grayScaleTexture, healthVariable);

	if (healthSeconds < 1.0f)
	{
		float healthVar = 1.0;
		healthVar = (healthSeconds) / 1.0;

		mixHealth = mix(grayScaleTexture, mixHealth, healthVar);
	}
	else if (healthSeconds >= 1.0f)
	{
		float healthVar = 1.0;
		healthVar = (healthSeconds-1) / 1.0;

		mixHealth = mix(mixHealth, grayScaleTexture, healthVar);
	}



	//healthColor = Albedo + healthColor;
	if (healthColor.r < 0.1 && healthColor.g < 0.1 && healthColor.b < 0.1)
		FragColor = grayScaleTexture;
	else
		FragColor = mixHealth.xyz;


	//vec3 finalTexture = mix(blurTexture, grayScaleTexture, thirstVariable);



}