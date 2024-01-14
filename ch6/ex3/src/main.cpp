#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <ostream>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec3 aColor;\n"
  "out vec3 vertexColor;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos, 1.0);\n"
  "   vertexColor = aColor;\n"
  "}";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 vertexColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(vertexColor, 1.0);\n"
  "}\n";

int main()
{
  // GLFW initialize
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Make GLFW window object
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Make sure GLAD loads
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Shader shader("data/shaders/shader.vs", "data/shaders/shader.fs");

  // VBO data
  // Triangle vertex input 
  float vertices[] = {
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // right 
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f     // top
  };

  /* Square vertex data
  float vertices[] = {
    0.5f, 0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f   // top left
  };
  unsigned int indicies[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
  };
  */

  // setup vertex data (and buffer(s)) and configure vertex attributes
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  //1. bind vertex array object 
  glBindVertexArray(VAO);
  // 2. copy our verticies array in a buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. then set our vertex attributes pointers
  // position attribute
  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // ..:: Drawing code (in render loop) ::..
  // 4. draw the object

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // rendering command here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* change shader over time
    float timeValue = glfwGetTime();
    float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
    if (vertexColorLocation == -1)
    {
      std::cout << "Could not find uniform in shader" << std::endl;
      return -1;
    }
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    */
    shader.use();
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Delete all arrays, buffers, and program
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

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
