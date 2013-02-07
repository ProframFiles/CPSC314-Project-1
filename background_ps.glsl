varying vec3 my_vertex;

uniform samplerCube my_enviro_map;

void main()
{
	gl_FragColor.a = 0.0;
	gl_FragColor.xyz = textureCube(my_enviro_map, my_vertex ).xyz;
	//gl_FragColor.xyz =my_vertex.xyz;
}

