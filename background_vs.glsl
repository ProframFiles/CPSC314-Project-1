varying vec3 my_vertex;

void main()
{
	gl_Position = ftransform();
	my_vertex =  ((gl_ModelViewMatrix*gl_Vertex).xyz);
}
