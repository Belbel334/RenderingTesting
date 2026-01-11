#include<iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

using namespace std;

GLfloat vertices[] = {

  0.25f, -0.25f,0.0f, //0
  -0.25f, -0.25f,0.0f,//1
  0.25f, 0.25f, 0.0f, //2
  -0.25f, 0.25f, 0.0f, //3

  0.55f, -0.25f,0.0f, //4
  0.30f, -0.25f, 0.0f, //5
  0.55f, 0.25f, 0.0f, //6
  0.30f, 0.25f, 0.0f //7
};

GLuint indices[] = {
  0,1,2,
  2,3,1,

  4,5,6,
  6,7,5
};
float colorL[3]={ 0.1f, 0.1f, 0.8f };
float colorR[3]  = { 0.8f, 0.1f, 0.1f };
bool LeftOn=false;
bool RightOn=false;
int buttonsOn=0;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float x = (2.0f * xpos) / width - 1.0f;
    float y = 1.0f - (2.0f * ypos) / height;

    // links
    if (x >= -0.25f && x <= 0.25f && y >= -0.25f && y <= 0.25f)
    {
      cout << "left" << endl;
      if(!LeftOn){
        LeftOn=!LeftOn;
        colorL[0]=0.9f;
        colorL[1]=0.9f;
        colorL[2]=1.0f;
        buttonsOn++;

      }else{
        LeftOn=!LeftOn;
        colorL[0]=0.1f;
        colorL[1]=0.1f;
        colorL[2]=0.8f;
        buttonsOn-=1;
      }
    }
    // recht
    else if (x >= 0.30f && x <= 0.55f && y >= -0.25f && y <= 0.25f)
    {
      cout << "right" << endl;
      if(!RightOn){
        RightOn=!RightOn;
        colorR[0]=1.0f;
        colorR[1]=0.9f;
        colorR[2]=0.9f;
        buttonsOn++;

      }else{
        RightOn=!RightOn;
        colorR[0]=0.8f;
        colorR[1]=0.1f;
        colorR[2]=0.1f;
        buttonsOn-=1;
      }
    }else cout << "none"<<endl;
    cout<<buttonsOn<<endl;
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(action==GLFW_PRESS){
    cout<<"pressed sum shit or sumn idk" << endl;
  } else if(action==GLFW_RELEASE){
    cout<<"let da key go or sumn idk" << endl;
  }
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


int main()
{

  glfwSetErrorCallback(error_callback);
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 800, "Supertofspelletje", NULL, NULL);
  if (window == NULL)
  {
    cout << "kutspelletje" << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetKeyCallback(window, key_callback);

  gladLoadGL();
  glViewport(0, 0, 800, 800);

  Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));

  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkVBO(VBO1, 0);
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();
  GLuint colorLLoc = glGetUniformLocation(shaderProgram.ID, "colorL");
  GLuint colorRLoc = glGetUniformLocation(shaderProgram.ID, "colorR");
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.Activate();

    glUniform3f(colorLLoc, colorL[0], colorL[1], colorL[2]);
    glUniform3f(colorRLoc, colorR[0], colorR[1], colorR[2]);

    VAO1.Bind();

    glDrawElements(GL_TRIANGLES, 10000, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shaderProgram.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
