#version 400
out vec4 fragcolor;           
in float vout_height;
in vec3 vout_normal;

uniform float slider;
uniform float time;

void main(void)
{   
	vec3 light_dir = normalize(vec3(sin(time*0.8),cos(time*0.8),0));
	vec3 light_color = vec3(1,0.6,0);
	float height = vout_height;
	vec3 result = vec3(1,0,1);
	if(height < 0.01f)	// water
	{
		result = vec3(0,0.5,1);
	}
	else if(height < 0.02f)	// water edge
	{
		result = vec3(0.4,0.7,1);
	}
	else if(height < 0.03f)	// dirt
	{
		result = vec3(0.3,0.3,0);
	}
	else	//grass
	{
		result = vec3(0.2,0.6,0);
	}
	vec3 ambient_mat, ambient_light, diffuse_mat, diffuse_light;
	ambient_mat = result*0.2;
	ambient_light = vec3(0.5,0.5,0.5);
	diffuse_mat = result;
	diffuse_light = light_color;
	result += ambient_mat*ambient_light + diffuse_mat*diffuse_light*dot(light_dir,vout_normal);
	fragcolor = vec4(result,1);
	
}