# About
## simple opengl project that includes:
two buttons that brighten up upon being clicked.  
Counter for how many clicked buttons  
Keyboard detection  

___

# compiling


### out file

g++ src/main.cpp src/glad.c src/shaderClass.cpp 
src/VAO.cpp src/VBO.cpp src/EBO.cpp -Iinclude -Iheaders $(pkg-config --cflags --li
bs glfw3) -lGL -ldl -lpthread -o out

### .exe file

x86_64-w64-mingw32-g++ main.cpp glad.c shaderClass.cpp VAO.cpp VBO.cpp EBO.cpp -o out.exe \
  -I./include \
  -L./lib-mingw-w64 \
  -lglfw3 -lopengl32 -lgdi32 -static


