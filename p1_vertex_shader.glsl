uniform vec2 pixel_nudge;
uniform vec2 viewport;
uniform float my_vertex_time; 

void main()
{
	my_vertex = gl_Normal.xyz;
	vec3 world_x = gl_NormalMatrix*gl_MultiTexCoord0.xyz;
	vec3 world_z = gl_NormalMatrix*gl_Normal;
	vec3 world_y = cross(world_z, world_x);

	// generate the matrix which takes our texture space normal and transforms to 
	// world coordinates (which is where we call reflect() )
	tangent_mapping = mat3(
		world_x.x, world_x.y, world_x.z,
		world_y.x, world_y.y, world_y.z,
		world_z.x, world_z.y, world_z.z
	);
	//gives us screen space coords equivalent to the fixed function pipeline
	
	vec4 world_vertex = gl_ModelViewMatrix*(gl_Vertex);
	float noise_fac = snoise(vec4(15.0*world_vertex.xyz, 1.0*my_vertex_time));
	world_vertex = gl_ModelViewMatrix*(gl_Vertex+vec4(0.0, 0.4*noise_fac, 0.0, 0.0));

	//world_vertex += 0.001*noise_fac;
	gl_Position = gl_ProjectionMatrix*world_vertex;
	vec4 adjusted_vert = gl_Vertex;
	

	//generate the texture coordinates from our vertex location
	vec3 object_y = cross(gl_Normal, gl_MultiTexCoord0.xyz);
	my_texcoord = vec2(dot(adjusted_vert.xyz, gl_MultiTexCoord0.xyz), dot(adjusted_vert.xyz, object_y))+0.5;
	my_texcoord.x += 0.5*noise_fac-(2.0)*my_vertex_time;
	//my_texcoord.x *= longness;
	//some varyings to pass through
	my_normal = world_z;
	eye_vec =  (gl_ModelViewMatrix*adjusted_vert).xyz ;

	vec3 screen_z = gl_NormalMatrix*gl_Normal;
}
