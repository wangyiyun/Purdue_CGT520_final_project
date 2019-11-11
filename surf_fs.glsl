#version 400
out vec4 fragcolor;           
in float vout_height;

uniform float slider;

void main(void)
{   
	float height = vout_height;
	if(height < 0.02f)
	{
		fragcolor = vec4(0,0.5,1, 1.0);
	}
	else if(height < 0.05f)
	{
		fragcolor = vec4(0.3,0.3,0, 1.0);
	}
	else if(height < 0.15f)
	{
		fragcolor = vec4(0.2,0.6,0, 1.0);
	}
	else
	{
		fragcolor = vec4(1,1,1, 1.0);
	}
}




















