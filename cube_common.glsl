varying vec2 my_texcoord;
varying vec3 my_vertex;
varying vec3 eye_vec;
varying vec3 my_normal;
varying mat3 tangent_mapping;


//bunch of convenience functions for math and lighting, mostly
vec3 rotate_by_quat(vec3 v, vec4 q)
{
	return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w*v );
}
vec3 reverse_rotate_by_quat(vec3 v, vec4 q)
{
	return v + 2.0 * cross(q.xyz, cross(q.xyz, v) - q.w*v );
}
vec3 tone_map(vec3 in_hdr)
{
	return in_hdr/(in_hdr+0.5);
}
float luminance(vec3 color)
{
	return dot(color, vec3(0.2126, 0.7152, 0.0722));
}
vec3 luminance_scale(vec3 color)
{
	return color * vec3(0.6378, 2.1456, 0.2166);
}
vec3 luminance_scale_inverse(vec3 color)
{
	return color * vec3( 1.5678896,  0.466070,  4.616805);
}
vec4 quaternion(vec3 from, vec3 to)
{
	vec3 m = normalize(from + to);
	return vec4( cross(m, to), dot(m, to) );
}
// blend mode equations from wikipedia

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

vec3 hard_light(vec3 color, vec3 light)
{
	return overlay(light, color);
}

vec3 soft_light(vec3 color, vec3 light)
{
	 return color*(2.0*light*(1.0-color) + color);
}

vec3 soft_luminance(vec3 color, vec3 light)
{
	return luminance_scale_inverse( soft_light(luminance_scale(color), light));
}

float whiteness(vec3 v)
{
	float small = min(min(v.x, v.y), v.z);
	float big = max(max(v.y, v.x),v.z);
	return 1.0-(big-small);
}
