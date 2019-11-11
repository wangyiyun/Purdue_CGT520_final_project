#version 400

uniform vec4 La = vec4(0.4);
uniform vec4 Ld = vec4(0.7);
uniform vec4 Ls = vec4(0.7);
uniform vec4 ks = vec4(0.5);

uniform float alpha = 20.0; //phong specular exponent (shininess)

const vec3 l = vec3(0.0, 1.0, 0.0); //world space (directional light)

uniform sampler2D texture;
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

   vec4 tex_color = texture2D(texture, tex_coord);

   //compute phong lighting in world space
   vec4 amb = tex_color*La;
   vec4 diff = tex_color*Ld*max(dot(n,l), 0.0);
   vec4 spec = ks*Ls*pow(max(dot(r,v), 0.0), alpha);

   fragcolor =  amb + diff + spec;

}




















