#version 400

out vec4 fragcolor;           
in vec3 pos;

uniform samplerCube cubemap;
uniform float time;

const vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 color2 = vec4(0.4, 0.4, 0.4, 1.0);
     
void main(void)
{   
	vec3 light_dir = normalize(vec3(sin(time*0.8),cos(time*0.8),0));
	vec3 light_color = vec3(0.1,0.2,0);
	vec3 n = vec3(0,1,0);

	vec4 result = texture(cubemap, pos);
	fragcolor = texture(cubemap, pos);

	vec3 ambient_mat, ambient_light, diffuse_mat, diffuse_light;
	ambient_mat = result.xyz*0.2;
	ambient_light = vec3(0.2,0.2,0.2);
	diffuse_mat = result.xyz;
	diffuse_light = light_color;
	result.xyz += ambient_mat*ambient_light + diffuse_mat*diffuse_light*dot(light_dir,n);
	fragcolor = result;
}




















