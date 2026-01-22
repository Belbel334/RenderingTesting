# About
## simple opengl project that includes:

- Small cube that moves with the WASD keys  
- Out of bounds protection
- Keyboard holding 
- escape key to quit
- kinda scuffed background
- terrain collision


___

# compiling


### out file
``` bash
g++ src/main.cpp src/glad.c src/shaderClass.cpp src/VAO.cpp src/VBO.cpp src/EBO.cpp -Iinclude -Iheaders -IKHR $(pkg-config --cflags --libs glfw3) -lGL -ldl -lpthread -o out
```

### .exe file
> [!NOTE]
> a bit weird

``` bash
x86_64-w64-mingw32-g++ src/main.cpp src/glad.c src/shaderClass.cpp src/VAO.cpp src/VBO.cpp src/EBO.cpp -o out.exe \
  -Iinclude \
  -Iheaders \
  -IKHR \
  -L./lib-mingw-w64 \
  -lglfw3 -lopengl32 -lgdi32 -static
```
