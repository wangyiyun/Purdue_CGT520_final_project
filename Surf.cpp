#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> //for pi
#include "Surf.h"

#include <iostream>
using namespace std;
using namespace glm;

static int N = 50;

//The surface to draw.
glm::vec3 surf(int i, int j)
{
   const float offset = 0.5f;
   const float step = 1.0f / N;
   const float z_scale = 10.0f;

   float x = -offset + step * i;
   float y = offset - step * j;

   float z = 0.0f;
   return glm::vec3(x, y, 0);
}

vec3 setColor(int n)
{
	switch (n)
	{
	case 1:
		return vec3(0);
		break;
	case 2:
		return vec3(0.25);
		break;
	case 3:
		return vec3(0.5);
		break;
	case 4:
		return vec3(1);
		break;
	default:
		break;
	}
}

GLuint create_surf_vbo()
{
   //Declare a vector to hold N vertices
   std::vector<glm::vec3> surf_verts(N*N*12);
   int count = 0;
   for(int i = 0; i < N; i++)
   {
	   //surf_verts[i] = circle(i);
	   for (int j = 0; j < N; j++)
	   {
		   if((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))
		   {
			   // use point (i,j)
			   surf_verts[count++] = surf(i, j);
			   surf_verts[count++] = surf(i,j) + vec3(0.25,0.25,0);
			   surf_verts[count++] = surf(i + 1, j);
			   surf_verts[count++] = surf(i, j) + vec3(0.25, 0.25, 0);
			   surf_verts[count++] = surf(i, j + 1);
			   surf_verts[count++] = surf(i, j) + vec3(0.25, 0.25, 0);

			   //
			   surf_verts[count++] = surf(i, j + 1);
			   surf_verts[count++] = surf(i+1, j + 1)- vec3(0.25, 0.25, 0);
			   surf_verts[count++] = surf(i + 1, j);
			   surf_verts[count++] = surf(i + 1, j + 1) - vec3(0.25, 0.25, 0);
			   surf_verts[count++] = surf(i + 1, j + 1);
			   surf_verts[count++] = surf(i + 1, j + 1) - vec3(0.25, 0.25, 0);
		   }
		   else
		   {
			   surf_verts[count++] = surf(i, j);
			   surf_verts[count++] = surf(i + 1, j)+ vec3(0.25, -0.25, 0);
			   surf_verts[count++] = surf(i + 1, j);
			   surf_verts[count++] = surf(i + 1, j) + vec3(0.25, -0.25, 0);
			   surf_verts[count++] = surf(i + 1, j + 1);
			   surf_verts[count++] = surf(i + 1, j) + vec3(0.25, -0.25, 0);

			   surf_verts[count++] = surf(i, j);
			   surf_verts[count++] = surf(i+1, j) + vec3(-0.25, 0.25, 0);
			   surf_verts[count++] = surf(i + 1, j + 1);
			   surf_verts[count++] = surf(i + 1, j) + vec3(-0.25, 0.25, 0);
			   surf_verts[count++] = surf(i, j + 1);
			   surf_verts[count++] = surf(i + 1, j) + vec3(-0.25, 0.25, 0);
		   }
	   }
   }

   GLuint vbo;
   glGenBuffers(1, &vbo); //Generate vbo to hold vertex attributes for triangle.

   glBindBuffer(GL_ARRAY_BUFFER, vbo); //Specify the buffer where vertex attribute data is stored.
   
   //Upload from main memory to gpu memory.
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*surf_verts.size(), surf_verts.data(), GL_STATIC_DRAW);
   return vbo;
}

GLuint create_surf_vao()
{
   GLuint vao;

   //Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
   glGenVertexArrays(1, &vao);

   //Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
   glBindVertexArray(vao);

   GLuint vbo = create_surf_vbo();

   const GLint pos_loc = 0; //See also InitShader.cpp line 164.

   glEnableVertexAttribArray(pos_loc); //Enable the position attribute.

   //Tell opengl how to get the attribute values out of the vbo (stride and offset).
   //glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 3*sizeof(float), 0);
	// Position attribute
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);
   // Color attribute
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1);
   glBindVertexArray(0); //unbind the vao

   return vao;
}

void draw_surf(GLuint vao, bool drawLine)
{
	if (drawLine)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	glDrawArrays(GL_TRIANGLES, 0, N * N * 6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
