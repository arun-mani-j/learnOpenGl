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

void createVertexArrayObject(GLuint *vertexArrayObj, GLuint *vertexBufferObj, //
                             size_t size, float vertices[]) {
  glGenVertexArrays(1, vertexArrayObj);
  glGenBuffers(1, vertexBufferObj);

  glBindVertexArray(*vertexArrayObj);

  glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferObj);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
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
  // T0
  +0.25f, +0.25f, 0.0f,
  +0.50f, +0.25f, 0.0f,
  +0.50f, +0.50f, 0.0f,
  // T1
  -0.50f, +0.25f, 0.0f,
  -0.25f, +0.25f, 0.0f,
  -0.50f, +0.50f, 0.0f,
  };
  // clang-format on
  unsigned int vertexArrayObjs[2];
  unsigned int vertexBufferObjs[2];

  createVertexArrayObject(&vertexArrayObjs[0], &vertexBufferObjs[0], 9, &vertices[0]);
  createVertexArrayObject(&vertexArrayObjs[1], &vertexBufferObjs[1], 9, &vertices[9]);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArrayObjs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(vertexArrayObjs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, vertexArrayObjs);
  glDeleteBuffers(2, vertexBufferObjs);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void onFramebufferSize(GLFWwindow *, int width, int height) { glViewport(0, 0, width, height); }
