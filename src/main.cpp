#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>


#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"

using namespace std;
using namespace chrono_literals;

GLfloat xpositions[] = {
    0.10f,
    -0.10f,
    0.10f,
    -0.10f,
};

GLfloat ypositions[] = {
    -0.10f,
    -0.10f,
    0.10f,
    0.10f,
};

GLfloat vertices[] = {
    xpositions[0], ypositions[0], 0.0f, // 0
    xpositions[1], ypositions[1], 0.0f, // 1
    xpositions[2], ypositions[2], 0.0f, // 2
    xpositions[3], ypositions[3], 0.0f, // 3

};

GLuint indices[] = {
    0, 1, 2, 2, 3, 1,

};

GLfloat terrainVerts[]={
  //floor
  -1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  0.0f, -1.1f, 0.0f, //triangle but it doesnt matter because you dont see the bottom(unsure if this is neededd but safety)
  
  -1.0f, -0.8f, 0.0f,
  -0.7f, -0.8f, 0.0f,
  -0.7f, -0.6f, 0.0f,
  -1.0f, -0.6f, 0.0f, //small box




};
GLuint terrainInds[]={
  0,1,2,// floor triangle

  3,4,5,
  5,6,3,

};

const int WorldGridSize=50;
const int vertCount = (WorldGridSize+1)*(WorldGridSize+1)*3;
const int indCount=WorldGridSize*WorldGridSize*6;

GLfloat worldVertices[vertCount];
GLuint worldIndices[indCount];

void Genworld(){
  int vertexIndex=0;
  float Cellsize=0.5f/WorldGridSize;

  for(int row=0; row<=WorldGridSize; row++){
    for(int col=0; col<=WorldGridSize;col++){
      worldVertices[vertexIndex++]=(-1.0f+col)*Cellsize;
      worldVertices[vertexIndex++]=(-1.0f+row)*Cellsize;  

      int White=(row+col)%2;
      worldVertices[vertexIndex++]=White ? 1.0f : 0.0f;
    }
  }
  int indexPos = 0;
  for (int row = 0; row < WorldGridSize; row++) {
    for (int col = 0; col < WorldGridSize; col++) {
      int topLeft = (row * (WorldGridSize + 1) + col);
      int topRight = topLeft + 1;
      int bottomLeft = ((row + 1) * (WorldGridSize + 1) + col);
      int bottomRight = bottomLeft + 1;
      
      worldIndices[indexPos++] = topLeft;
      worldIndices[indexPos++] = bottomLeft;
      worldIndices[indexPos++] = topRight;
      
      worldIndices[indexPos++] = topRight;
      worldIndices[indexPos++] = bottomLeft;
      worldIndices[indexPos++] = bottomRight;
    }
  }
}
float colorL[3] = {0.1f, 0.1f, 0.8f};

float Yvel=0.0f;
float g = -0.001f;
float bounciness = 0.5f;
bool grounded = false;

void updatePos() {
  float oldXPositions[4], oldYPositions[4];
  for(int i = 0; i < 4; i++) {
    oldXPositions[i] = xpositions[i];
    oldYPositions[i] = ypositions[i];
  }
  
  //Out of bounds
  if (xpositions[0] >= 1.0f) {
    xpositions[0] = 1.0f;
    xpositions[1] = 0.8f;
    xpositions[2] = 1.0f;
    xpositions[3] = 0.8f;
  }
  
  // Out of bounds
  if (xpositions[1] <= -1.0f) {
    xpositions[0] = -0.8f;
    xpositions[1] = -1.0f;
    xpositions[2] = -0.8f;
    xpositions[3] = -1.0f;
  }
  
  // Out of bounds
  if (ypositions[0] <= -1.0f) {
    ypositions[0] = -1.0f;
    ypositions[1] = -1.0f;
    ypositions[2] = -0.8f;
    ypositions[3] = -0.8f;
    if (Yvel < 0) {
      Yvel = -Yvel * bounciness;
      if (abs(Yvel) < 0.001f) {
        Yvel = 0.0f;
        grounded = true;
      }
    }
  } else {
    grounded = false;
  }
  
  // Same old
  if (ypositions[2] >= 1.0f) {
    ypositions[0] = 0.8f;
    ypositions[1] = 0.8f;
    ypositions[2] = 1.0f;
    ypositions[3] = 1.0f;
    if(Yvel > 0) {
      Yvel = -Yvel * bounciness;
    }
  }
  
  // BOX elements UPDATE to read array later in case of big maps
  float boxLeft = -1.0f;
  float boxRight = -0.7f;
  float boxBottom = -0.8f;
  float boxTop = -0.6f;
  //for (blabe you know 0,3,6... wil be X pos, 1,4,7... will be Y pos)
  //check for each Y pos take which one it did and overlap with that one?
  //
   bool overlapping = !(xpositions[0] < boxLeft ||   
                       xpositions[1] > boxRight ||   
                       ypositions[2] < boxBottom ||  
                       ypositions[0] > boxTop);      
  
  if (overlapping) {
    float overlapLeft = xpositions[0] - boxLeft;    
    float overlapRight = boxRight - xpositions[1];  
    float overlapBottom = ypositions[2] - boxBottom;
    float overlapTop = boxTop - ypositions[0];      
    
    float minOverlap = overlapLeft;
    int direction = 0; 
    
    if (overlapRight < minOverlap) {
      minOverlap = overlapRight;
      direction = 1;
    }
    if (overlapBottom < minOverlap) {
      minOverlap = overlapBottom;
      direction = 2;
    }
    if (overlapTop < minOverlap) {
      minOverlap = overlapTop;
      direction = 3;
    }
    
    float playerWidth = xpositions[0] - xpositions[1];
    float playerHeight = ypositions[2] - ypositions[0];
    
    switch(direction) {
        xpositions[1] = boxLeft;
        xpositions[0] = boxLeft + playerWidth;
        xpositions[3] = boxLeft;
        xpositions[2] = boxLeft + playerWidth;
        break;
      case 1:
        xpositions[0] = boxRight;
        xpositions[1] = boxRight - playerWidth;
        xpositions[2] = boxRight;
        xpositions[3] = boxRight - playerWidth;
        break;
      case 2: 
        ypositions[2] = boxBottom;
        ypositions[3] = boxBottom;
        ypositions[0] = boxBottom - playerHeight;
        ypositions[1] = boxBottom - playerHeight;
        if (Yvel > 0) {
          Yvel = -Yvel * bounciness;
        }
        break;
      case 3: 
        ypositions[0] = boxTop;
        ypositions[1] = boxTop;
        ypositions[2] = boxTop + playerHeight;
        ypositions[3] = boxTop + playerHeight;
        if (Yvel < 0) {
          Yvel = -Yvel * bounciness;
          if (abs(Yvel) < 0.001f) {
            Yvel = 0.0f;
            grounded = true;
          }
        }
        break;
    }
  }
  
  vertices[0] = xpositions[0];
  vertices[1] = ypositions[0];
  vertices[3] = xpositions[1];
  vertices[4] = ypositions[1];
  vertices[6] = xpositions[2];
  vertices[7] = ypositions[2];
  vertices[9] = xpositions[3];
  vertices[10] = ypositions[3];
}

void applyGravity() {
  // Apply gravity acceleration to velocity
  Yvel += g;
  //cout << "gravity function, yvel= " << Yvel<<endl;
  
  for (int i = 0; i < 4; i++) {
    ypositions[i] += Yvel;
  }
  
  updatePos();
}
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float x = (2.0f * xpos) / width - 1.0f;
    float y = 1.0f - (2.0f * ypos) / height;

    cout << "pressed" << endl;
  }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_W || key == GLFW_KEY_SPACE || key == GLFW_KEY_UP) {
      if(grounded){
        Yvel = 0.03f;  // Simple upward velocity impulse
      }
    } else if (key == GLFW_KEY_S || key==GLFW_KEY_DOWN) {
      // Down movement if needed
      for (int i = 0; i < 4; i++) {
        ypositions[i] -= 0.05f;
      }
    } else if (key == GLFW_KEY_A || key==GLFW_KEY_LEFT) {
      for (int i = 0; i < 4; i++) {
        xpositions[i] -= 0.05f;
      }
    } else if (key == GLFW_KEY_D||key==GLFW_KEY_RIGHT) {
      for (int i = 0; i < 4; i++) {
        xpositions[i] += 0.05f;
      }
    }
    updatePos();
  } else if (action == GLFW_REPEAT) {
    if (key == GLFW_KEY_W || key == GLFW_KEY_SPACE || key == GLFW_KEY_UP) {
      if(grounded){
        Yvel = 0.03f;  // Simple upward velocity impulse
      }
    } else if (key == GLFW_KEY_A || key==GLFW_KEY_LEFT) {
      for (int i = 0; i < 4; i++) {
        xpositions[i] -= 0.05f;
      }
    } else if (key == GLFW_KEY_D||key==GLFW_KEY_RIGHT) {
      for (int i = 0; i < 4; i++) {
        xpositions[i] += 0.05f;
      }
    }
    updatePos();
  }
}

void error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {

  glfwSetErrorCallback(error_callback);
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  GLFWwindow *window =
      glfwCreateWindow(800, 800, "Supertofspelletje", NULL, NULL);
  if (window == NULL) {
    cerr << "kutspelletje" << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetKeyCallback(window, key_callback);

  gladLoadGL();
  glViewport(0, 0, 800, 800);

  Genworld();

  Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

  updatePos();

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));

  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkVBO(VBO1, 0);
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  VAO worldVAO;
  worldVAO.Bind();
  VBO worldVBO(worldVertices, sizeof(worldVertices));
  EBO worldEBO(worldIndices, sizeof(worldIndices));
  worldVAO.LinkVBO(worldVBO, 0);
  worldVAO.Unbind();
  worldVBO.Unbind();
  worldEBO.Unbind();

  VAO terrainVAO;
  terrainVAO.Bind();
  VBO terrainVBO(terrainVerts, sizeof(terrainVerts));
  EBO terrainEBO(terrainInds, sizeof(terrainInds));
  terrainVAO.LinkVBO(terrainVBO,0);
  terrainVAO.Unbind();
  terrainVAO.Unbind();
  terrainEBO.Unbind();
  cout << "color ID " << shaderProgram.ID << endl;
  GLuint colorLLoc = glGetUniformLocation(shaderProgram.ID, "colorL");
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    applyGravity();

    glUniform3f(colorLLoc, 0.2f, 0.7f, 0.3f);
    worldVAO.Bind();
    glDrawElements(GL_TRIANGLES, 1000000, GL_UNSIGNED_INT, 0);
    worldVAO.Unbind();

    VBO1.Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    VBO1.Unbind();

    glUniform3f(colorLLoc, 0.9f, 0.9f, 0.9f);
    terrainVAO.Bind();
    glDrawElements(GL_TRIANGLES, 1000000, GL_UNSIGNED_INT, 0);
    terrainVAO.Unbind();

    shaderProgram.Activate();
    glUniform3f(colorLLoc, colorL[0], colorL[1], colorL[2]);
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, 10000, GL_UNSIGNED_INT, 0);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
      cout << "quit game"<<endl;
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  worldVAO.Delete();
  worldVBO.Delete();
  worldEBO.Delete();
  terrainVAO.Delete();
  terrainVBO.Delete();
  terrainEBO.Delete();

  shaderProgram.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
