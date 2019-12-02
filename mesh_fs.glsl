#version 400

uniform vec4 La = vec4(0.4);
uniform vec4 Ld = vec4(0.7);
uniform vec4 Ls = vec4(0.7);
uniform vec4 ks = vec4(0.5);

uniform float alpha = 20.0; //phong specular exponent (shininess)

const vec3 l = vec3(0.0, 1.0, 0.0); //world space (directional light)

uniform samplerCube cubemap;
uniform float slider;
uniform float time;

out vec4 fragcolor;           
in vec2 tex_coord;

in vec3 normal;   //World-space normal vector
in vec3 p;        //World-space fragment position
in vec3 eye;      //World-space eye position
     
void main(void)
{   
   vec3 n = normalize(normal); // unit normal vector
   vec3 v = normalize(eye-p); // unit view vector
   vec3 r = reflect(-l, n); // unit reflection vector

   if(n.y < 0) n.y *= -1;

   vec3 light_dir = normalize(vec3(sin(time*0.5),cos(time*0.5),0));
	vec3 light_color = vec3(1,1,1);

   vec3 result = vec3(1) + vec3(0.5)*light_color*dot(n,light_dir);

   fragcolor =  vec4(result,0.75);

}




















