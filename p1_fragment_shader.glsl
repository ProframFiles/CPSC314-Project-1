//>! uniform diffuse_sampler 
//>! uniform topology_sampler "alien_nm.tga"
//>! uniform light_params_sampler "alien_ao.tga"
uniform sampler2D diffuse_sampler;
uniform sampler2D topology_sampler;
uniform sampler2D light_params_sampler;
uniform samplerCube detailed_cubemap;
uniform samplerCube diffuse_cubemap;


void main()
{
	if(my_vertex.x < 0.2 )
	{
		discard;
		//gl_FragColor.a = 1.0;
	}
	else
	{
		gl_FragColor.a = 1.0;
	}
	vec4 normal_sample = texture2D(topology_sampler, my_texcoord);

	vec3 tex_normal = tangent_mapping*vec3(normal_sample.xy-0.5, normal_sample.z);

	vec3 cube_lookup = reflect(eye_vec, my_normal);
	vec3 reflect_color = textureCube(detailed_cubemap, cube_lookup).xyz;
	cube_lookup = tex_normal;
	vec3 diffuse_reflection = textureCube(diffuse_cubemap, cube_lookup).xyz;

	vec4 dif_sample = texture2D(diffuse_sampler, my_texcoord);
	vec4 misc = texture2D(light_params_sampler, my_texcoord);

	// for debugging
	//gl_FragColor.xyz = my_vertex;
	//for real
	gl_FragColor.xyz = overlay(reflect_color, dif_sample.xyz);

}
