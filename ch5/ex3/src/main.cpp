#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// vertex shader data
const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}";

// fragment shader data
const char *orangeFragShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}";

const char *yellowFragShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
  "}";

int main()
{
  // GLFW initialize
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Make GLFW window object
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Double Triangles, Charlie!", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "GLFW Window failed to create!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Load GLAD 
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed in initialize GLAD" << std::endl;
    return -1;
  }

  // Compile vertex shader program(s)
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Error check shader
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Compile orange fragment shader program(s)
  unsigned int orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(orangeFragmentShader, 1, &orangeFragShaderSource, NULL);
  glCompileShader(orangeFragmentShader);
  // Error check fragment shader
  glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(orangeFragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Compile yellow fragment shader program
  unsigned int yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(yellowFragmentShader, 1, &yellowFragShaderSource, NULL);
  glCompileShader(yellowFragmentShader);
  // Error check fragment shader
  glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::YELLOW::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Create the shader program
  unsigned int yellowShaderProgram = glCreateProgram();
  unsigned int orangeShaderProgram = glCreateProgram();
  glAttachShader(orangeShaderProgram, vertexShader);
  glAttachShader(orangeShaderProgram, orangeFragmentShader);
  glLinkProgram(orangeShaderProgram);
  glAttachShader(yellowShaderProgram, vertexShader);
  glAttachShader(yellowShaderProgram, yellowFragmentShader);
  glLinkProgram(yellowShaderProgram);

  // Error check the shader program
  glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(orangeShaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::ORANGE::LINKING_ERROR\n" << infoLog << std::endl;
  }
  glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(yellowShaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::YELLOW::LINKING_ERROR\n" << infoLog << std::endl;
  }

  // Delete the shader objects
  glDeleteShader(vertexShader);
  glDeleteShader(orangeFragmentShader);
  glDeleteShader(yellowFragmentShader);

  // Vertex data
  float firstTriangle[] = {
    -1.0f, -0.5, 0.0f,  // left
    0.0f, -0.5f, 0.0f,  // right
    -0.5f, 0.5f, 0.0f   // top
  };
  float secondTriangle[] = {
    0.0f, -0.5f, 0.0f,  // left
    1.0f, -0.5f, 0.0f,  // right
    0.5f, 0.5f, 0.0f    // top
  };
  
  // Setup vertex data (and buffer(s)) and configure vertex attributes
  // 1. bind vertex array
  unsigned int VBO[2], VAO[2];
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);

  // 2. copy our verticies array in a buffer for OpenGL to use
  // first triangle
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // second triangle
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // ..:: Drawing code (in render loop) ::..
  // 4. draw the object
  // render loop
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // draw commands
    glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(orangeShaderProgram);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(yellowShaderProgram);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // check and call events and swap the buffers

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // Delete all arrays, buffers, and program
  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);
  glDeleteProgram(yellowShaderProgram);
  glDeleteProgram(orangeShaderProgram);

  // Terminate GLFW
  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
