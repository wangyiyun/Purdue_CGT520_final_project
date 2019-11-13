#version 400
out vec4 fragcolor;           
in float vout_height;
in vec3 vout_normal;

uniform float slider;

void main(void)
{   
	float height = vout_height;
	vec3 result = vec3(1,0,1);
	if(height < 0.02f)
	{
		result = vec3(0,0.5,1);
	}
	else if(height < 0.05f)
	{
		result = vec3(0.3,0.3,0);
	}
	else if(height < 0.15f)
	{
		result = vec3(0.2,0.6,0);
	}
	else
	{
		result = vec3(1,1,1);
	}

	result += dot(vec3(1,1,1),vout_normal)*0.1 + 0.2;
	fragcolor = vec4(result,1);
	
}