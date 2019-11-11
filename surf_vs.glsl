#version 400            
uniform mat4 PVM;

layout (location = 0) in vec3 pos_attrib; //in object space
layout (location = 1) in vec3 color_attrib;
out float vout_height;

uniform float slider;

const float PI = 3.1415926;

float rand3D(in vec3 co){
	return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
}

vec3 getNormal(float n)
{
	float offset = 1.0f/20;
	float h0, h1, h2;
	vec3 normal, v1, v2;
	h0 = rand3D(pos_attrib);
	//	0/|\2
	//	/1|3\
	if(n == 0)
	{
		h1 = rand3D(pos_attrib + vec3(offset,0,0));
		h2 = rand3D(pos_attrib + vec3(0,offset,0));
		v1 = vec3(offset,0,h1-h0);
		v2 = vec3(0,offset,h2-h0);
	}
	if(n == 0.25)
	{
		h1 = rand3D(pos_attrib + vec3(-offset,offset,0));
		h2 = rand3D(pos_attrib + vec3(0,offset,0));
		v1 = vec3(-offset,offset,h1-h0);
		v2 = vec3(0,offset,h2-h0);
	}
	if(n == 0.5)
	{
		h1 = rand3D(pos_attrib + vec3(-offset,0,0));
		h2 = rand3D(pos_attrib + vec3(0,offset,0));
		v1 = vec3(-offset,0,h1-h0);
		v2 = vec3(0,offset,h2-h0);
	}
	if(n == 1)
	{
		h1 = rand3D(pos_attrib + vec3(0,offset,0));
		h2 = rand3D(pos_attrib + vec3(offset,offset,0));
		v1 = vec3(0,offset,h1-h0);
		v2 = vec3(offset,offset,h2-h0);
	}
	normal = normalize(cross(v1,v2));
	return normal;
}

float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 p, float freq ){
	float unit = 10/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(PI*xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res){
	float persistance = .5;
	float n = 0.;
	float normK = 0.;
	float f = 4.;
	float amp = 1.;
	int iCount = 0;
	for (int i = 0; i<50; i++){
		n+=amp*noise(p, f);
		f*=2.;
		normK+=amp;
		amp*=persistance;
		if (iCount == res) break;
		iCount++;
	}
	float nf = n/normK;
	return nf*nf*nf*nf;
}


void main(void)
{
	vec3 offset = vec3(0,0, 0.5*pNoise(10*pos_attrib.xy+slider,30));
   //Compute clip-space vertex position
   gl_Position = PVM*vec4(pos_attrib + offset, 1.0);     //w = 1 becase this is a point

   vout_height = offset.z;
   gl_PointSize = 3.0f;
}
