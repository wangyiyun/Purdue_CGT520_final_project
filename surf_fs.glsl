#version 400
out vec4 fragcolor;           
in vec3 vout_normal;

uniform float slider;

void main(void)
{   
	fragcolor = vec4(vout_normal, 1.0);
}




















