#version 400            
uniform mat4 PVM;

layout (location = 0) in vec3 pos_attrib; //in object space
layout (location = 1) in vec3 color_attrib;
out float vout_height;
out vec3 vout_normal;

uniform float slider;
uniform float time;
uniform float mountHeight;

const float PI = 3.1415926;

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

float getHeight(vec2 uv)
{
	return 0.5*pNoise(10*uv,30) * mountHeight;
	//return 0.1*sin(10*uv.x)*cos(10*uv.y);
}

float getWaterHeight(vec2 pos)
{
	return 0.002*(sin(10*pos.x + time)*
		cos(10*pos.x + 15*pos.y + time*1.5));
}

vec3 getNormal(float h0, bool isWater)
{
	float offset;
	if(color_attrib.z != 0)
	{
		offset = 1.0f/100;
	}
	else offset = -1.0f/100;
	
	float h1, h2;
	vec2 p1,p2;
	vec3 normal, v1, v2;

	p1 = color_attrib.xy+slider + vec2(offset,0);
	p2 = color_attrib.xy+slider + vec2(0,offset);

	h1 = getHeight(p1);
	h2 = getHeight(p2);

	if(isWater)
	{
		h1 += getWaterHeight(p1);
		h2 += getWaterHeight(p2);
	}
	
	v1 = normalize(vec3(offset,h1-h0,0));
	v2 = normalize(vec3(0,h2-h0,offset));
	normal = normalize(cross(v2,v1));
	return normal;
}

void main(void)
{
	vec3 offset = vec3(0,0,getHeight(pos_attrib.xy + slider));
	float centerHeight = getHeight(color_attrib.xy + slider);
	bool isWater = false;
	//Compute clip-space vertex position
	if((pos_attrib+offset).z < 0.03f)
	{
		offset = vec3(0,0,getWaterHeight(pos_attrib.xy));
		isWater = true;
	}
	vec3 n = getNormal(centerHeight, isWater);
	gl_Position = PVM*vec4(pos_attrib+offset, 1.0);     //w = 1 becase this is a point
	
	vout_height = offset.z;
	vout_normal = n;
	gl_PointSize = 3.0f;
}