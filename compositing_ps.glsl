varying vec2 my_texcoord;
varying vec3 my_pos;
uniform float my_time; 
uniform sampler2D fbo_texture;

vec3 screen(vec3 color, vec3 light )
{
	return 1.0 - (1.0 - color)*(1.0 - light);
}

vec3 overlay(vec3 color, vec3 light)
{
	vec3 ba = 2.0*light*color;
	vec3 s = sign(ba-color);
	return color+s*min(abs(ba-color), abs(2.0*screen(color, light)-color));
}

vec3 get_color(float t)
{
	float tsq = t*t;
	vec3 low = mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 0.6, 1.0), tsq);
	vec3 high = mix(vec3(0.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), tsq);
	return  mix(low, high, tsq);
}

void main()
{
	vec3 strength = vec3(1.0) - abs(my_pos);
	vec4 sampled = texture2D(fbo_texture, my_texcoord );
	float noise = snoise(vec4((my_texcoord-vec2(0.5))*10.0,1.3*my_time, 2.0*my_time));
	vec4 sample_1 = texture2D(fbo_texture, my_texcoord + vec2(0.002*noise, 0.003*(1.0-my_texcoord.y)*1.6) );
	vec4 sample_2 = texture2D(fbo_texture, my_texcoord + vec2(0.003*noise, 0.003*(0.7+0.3*noise)) );
	vec4 sample_shifted = mix(sample_1, sample_2, 0.5 );
	float alpha = min(sample_shifted.a, 1.0);
	alpha = max(alpha, min((noise - 0.6)*3.0, 1.10));
	alpha = max(alpha, (1.0-noise)*0.2);
	gl_FragColor.xyz = get_color(alpha);// sample_shifted.xyz;
	gl_FragColor.a = (alpha*0.97);
}

