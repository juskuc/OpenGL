#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;

void main()
{
	//fs_color = vec4(vs_color, 1.f);
	//fs_color = texture(texture0, vs_textcoord) * texture(texture1, vs_textcoord) * vec4(vs_color, 1.f);
	//fs_color = mix(texture(texture0, vs_textcoord), texture(texture1, vs_textcoord), 0.67);
	
	// Ambient light
	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

	// Diffuse light
	vec3 posToLightDirVec = normalize(vs_position - lightPos0);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;

	fs_color = 
	texture(texture0, vs_textcoord)
	* vec4(vs_color, 1.f)
	* (vec4(ambientLight, 1.f)
	+ vec4(diffuseFinal, 1.f));

	//fs_color = vec4(1.f, 1.f, 1.f, 1.f);
}