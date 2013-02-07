varying vec2 my_texcoord;
varying vec3 my_pos;

void main()
{
	vec3 world_x = gl_NormalMatrix*gl_MultiTexCoord0.xyz;
	vec3 world_z = gl_NormalMatrix*gl_Normal;
	vec3 world_y = cross(world_z, world_x);

	// generate the matrix which takes our texture space normal and transforms to 
	// world coordinates (which is where we call reflect() )
	//gives us screen space coords equivalent to the fixed function pipeline
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	//gl_Position.xy += pixel_nudge;
	vec4 adjusted_vert = gl_ModelViewProjectionMatrixInverse*gl_Position;

	//generate the texture coordinates from our vertex location
	vec3 object_y = cross(gl_Normal, gl_MultiTexCoord0.xyz);
	my_texcoord = vec2(dot(adjusted_vert.xyz, gl_MultiTexCoord0.xyz), dot(adjusted_vert.xyz, object_y))+0.5;
	my_pos = world_z;//(gl_ModelViewMatrix*gl_Vertex).xy;
}
