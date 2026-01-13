#include<iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

using namespace std;

GLfloat xpositions[]={
  0.25f, -0.25f, 0.25f, -0.25f,
};

GLfloat ypositions[]={
  -0.25f, -0.25f,0.25f,0.25f,
};

GLfloat vertices[] = {
  xpositions[0], ypositions[0], 0.0f, //0
  xpositions[1], ypositions[1], 0.0f,//1
  xpositions[2], ypositions[2], 0.0f, //2
  xpositions[3], ypositions[3], 0.0f, //3

};

GLuint indices[] = {
  0,1,2,
  2,3,1,

};
float colorL[3]={ 0.1f, 0.1f, 0.8f };
float colorR[3]  = { 0.8f, 0.1f, 0.1f };
bool LeftOn=false;
bool RightOn=false;
int buttonsOn=0;
int pos[2]= {0,0};

void updatePos(){
  vertices[0] = xpositions[0];
    vertices[1] = ypositions[0];

    vertices[3] = xpositions[1];
    vertices[4] = ypositions[1];

    vertices[6] = xpositions[2];
    vertices[7] = ypositions[2];

    vertices[9] = xpositions[3];
    vertices[10] = ypositions[3];
}

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
      
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(action==GLFW_PRESS){
    if(key==GLFW_KEY_W){
      pos[1]++;
      for(int i=0; i<sizeof(ypositions);i++){
        ypositions[i]+=0.1f;
      } 
    }else if(key==GLFW_KEY_S){
      pos[1]-=1;
      for(int i=0; i<sizeof(ypositions);i++){
        ypositions[i]-=0.1f;
      }
    } else if(key==GLFW_KEY_A){
      pos[0]-=1;
      for(int i=0; i<sizeof(xpositions);i++){
        xpositions[i]-=0.1f;
      }
    }else if(key==GLFW_KEY_D){
      pos[0]++;
      for(int i=0; i<sizeof(xpositions);i++){
        xpositions[i]+=0.1f;
      }
      
    }
    updatePos();
    cout<<"pressed sum shit or sumn idk" << endl;
    cout << pos[0] << ","<< pos[1]<< endl;
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



    VBO1.Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    VBO1.Unbind();

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
