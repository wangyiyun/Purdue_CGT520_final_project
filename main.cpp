#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "imgui_impl_glut.h"

#include "Cube.h"
#include "Surf.h"


//Texture files and IDs
static const std::string texture_name = "BenizaT.bmp";
GLuint texture_id = -1; //Texture map for fish

static const std::string cube_name = "cubemap";
GLuint cubemap_id = -1; //Texture id for cubemap

//Mesh files and IDs
static const std::string vertex_shader("mesh_vs.glsl");
static const std::string fragment_shader("mesh_fs.glsl");
GLuint mesh_shader_program = -1;
static const std::string mesh_name = "Benizake0.obj";
MeshData mesh_data;

bool mesh_enabled = false;

//Surf files and IDs
static const std::string surf_vs("surf_vs.glsl");
static const std::string surf_fs("surf_fs.glsl");
GLuint surf_shader_program = -1;
GLuint surf_vao = -1;

//Cube files and IDs
static const std::string cube_vs("cube_vs.glsl");
static const std::string cube_fs("cube_fs.glsl");
GLuint cube_shader_program = -1;
GLuint cube_vao = -1;


//camera and viewport
float camangle = 0.0f;
glm::vec3 campos(0.0f, 1.0f, 2.0f);
float aspect = 1.0f;

// my var
bool m_drawLine =  false;
float slider = 1;

void draw_gui()
{
   glUseProgram(mesh_shader_program);
   static bool first_frame = true;
   ImGui_ImplGlut_NewFrame();

   ImGui::Begin("My GUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

   ImGui::Checkbox("Draw mesh", &mesh_enabled); ImGui::SameLine();
   ImGui::SliderFloat3("Cam Pos", &campos[0], -20.0f, +20.0f);
   ImGui::SliderFloat("Cam Angle", &camangle, -180.0f, +180.0f);
   ImGui::Checkbox("Draw Line", &m_drawLine);
   ImGui::SliderFloat("slider", &slider, 1.0f, +2.0f);
   ImGui::End();

   ImGui::Render();
   first_frame = false;
}

void draw_fish(const glm::mat4& P, const glm::mat4& V)
{
   glm::mat4 R = glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
   glm::mat4 M = R*glm::scale(glm::vec3(2.0f*mesh_data.mScaleFactor));
   
   glUseProgram(mesh_shader_program);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   int tex_loc = glGetUniformLocation(mesh_shader_program, "texture");
   if (tex_loc != -1)
   {
      glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
   }

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
   int cube_loc = glGetUniformLocation(mesh_shader_program, "cubemap");
   if (cube_loc != -1)
   {
      glUniform1i(cube_loc, 1); // we bound our texture to texture unit 1
   }

   int PVM_loc = glGetUniformLocation(mesh_shader_program, "PVM");
   if (PVM_loc != -1)
   {
      glm::mat4 PVM = P*V*M;
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }

   int M_loc = glGetUniformLocation(mesh_shader_program, "M");
   if (M_loc != -1)
   {
      glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
   }

   int V_loc = glGetUniformLocation(mesh_shader_program, "V");
   if (V_loc != -1)
   {
      glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
   }

   glBindVertexArray(mesh_data.mVao);
   mesh_data.DrawMesh();
}

void draw_cube(const glm::mat4& P, const glm::mat4& V)
{
   glUseProgram(cube_shader_program);
   int PVM_loc = glGetUniformLocation(cube_shader_program, "PVM");
   if (PVM_loc != -1)
   {
      glm::mat4 Msky = glm::scale(glm::vec3(5.0f));
      glm::mat4 PVM = P*V*Msky;
      PVM[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }
   
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
   int cube_loc = glGetUniformLocation(cube_shader_program, "cubemap");
   if (cube_loc != -1)
   {
      glUniform1i(cube_loc, 1); // we bound our texture to texture unit 1
   }

   glDepthMask(GL_FALSE);
   glBindVertexArray(cube_vao);
   draw_cube(cube_vao);
   glDepthMask(GL_TRUE);
}

void draw_surf(const glm::mat4& P, const glm::mat4& V)
{
   glUseProgram(surf_shader_program);
   int PVM_loc = glGetUniformLocation(surf_shader_program, "PVM");
   if (PVM_loc != -1)
   {
      glm::mat4 R = glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
      glm::mat4 M = R * glm::scale(glm::vec3(5.0f));
      glm::mat4 PVM = P*V*M;
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }
   int slider_loc = glGetUniformLocation(surf_shader_program, "slider");
   if (slider_loc != -1)
   {
	   glUniform1f(slider,slider);
   }
   glBindVertexArray(surf_vao);
   draw_surf(surf_vao, m_drawLine);
}

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
   
   glm::mat4 V = glm::lookAt(campos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))*glm::rotate(camangle, glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(80.0f, aspect, 0.1f, 100.0f); //not affine

   draw_cube(P, V);

   if(mesh_enabled)
   {
      draw_fish(P, V);
   }

   draw_surf(P, V);

   draw_gui();
   glutSwapBuffers();
}

void idle()
{
   glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;

   glUseProgram(mesh_shader_program);
   int time_loc = glGetUniformLocation(mesh_shader_program, "time");
   if (time_loc != -1)
   {
      //double check that you are using glUniform1f
      glUniform1f(time_loc, time_sec);
   }
}

void printGlInfo()
{
   std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
   std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
   std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void initOpenGl()
{
   glewInit();

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_POINT_SPRITE);       // allows textured points
   glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader
   glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

   cubemap_id = LoadCube(cube_name);

   mesh_shader_program = InitShader(vertex_shader.c_str(), fragment_shader.c_str());
   //mesh and texture to be rendered
   mesh_data = LoadMesh(mesh_name);
   texture_id = LoadTexture(texture_name);

   cube_shader_program = InitShader(cube_vs.c_str(), cube_fs.c_str());
   cube_vao = create_cube_vao();

   surf_shader_program = InitShader(surf_vs.c_str(), surf_fs.c_str());
   surf_vao = create_surf_vao();

   ImGui_ImplGlut_Init(); // initialize the imgui system
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);
}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
   ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
}

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
}

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   aspect = (float)w / h;
}

int main(int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition(5, 5);
   glutInitWindowSize(640, 640);
   int win = glutCreateWindow("Yiyun Final Project");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutIdleFunc(idle);
   glutReshapeFunc(reshape);

   initOpenGl();

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;
}