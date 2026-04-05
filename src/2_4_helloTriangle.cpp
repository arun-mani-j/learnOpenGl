#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <iostream>

void onFramebufferSize(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core

out vec4 fragColor;

void main() {
  fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

void checkSuccess(GLuint shader, GLenum pname) {
  int success;
  char buf[512];

  glGetShaderiv(shader, pname, &success);
  if (success)
    return;

  glGetShaderInfoLog(shader, sizeof(buf), NULL, buf);
  std::cerr << "Error:\n" << buf << std::endl;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, NAME, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, onFramebufferSize);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  checkSuccess(vertexShader, GL_COMPILE_STATUS);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  checkSuccess(fragmentShader, GL_COMPILE_STATUS);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  checkSuccess(shaderProgram, GL_LINK_STATUS);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // clang-format off
  float vertices[] = {
  +0.5f, +0.5f, 0.0f,
  +0.5f, -0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  -0.5f, +0.5f, 0.0f
  };
  unsigned int indices[] = {
  0, 1, 3,
  1, 2, 3
  };
  // clang-format on

  unsigned int vertexBufferObj, vertexArrayObj, elementBufferObj;
  glGenVertexArrays(1, &vertexArrayObj);
  glGenBuffers(1, &vertexBufferObj);
  glGenBuffers(1, &elementBufferObj);

  glBindVertexArray(vertexArrayObj);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArrayObj);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, (void *)0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertexArrayObj);
  glDeleteBuffers(1, &vertexBufferObj);
  glDeleteBuffers(1, &elementBufferObj);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void onFramebufferSize(GLFWwindow *, int width, int height) { glViewport(0, 0, width, height); }
