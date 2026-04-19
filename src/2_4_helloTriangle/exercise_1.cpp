#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <print>
#include <vector>

void onFramebufferSize(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const auto vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const auto fragmentShaderSource = R"(
#version 330 core

out vec4 fragColor;

void main() {
  fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

void checkShaderStatus(GLuint shader, GLenum pname) {
  GLint success = 0;

  glGetShaderiv(shader, pname, &success);
  if (success == 1) {
    return;
  }

  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  std::vector<char> info(length);

  glGetShaderInfoLog(shader, length, nullptr, info.data());
  std::println(stderr, "Error:\n{}", std::string_view(info.data(), info.size()));
}

void checkProgramStatus(GLuint program, GLenum pname) {
  GLint success = 0;

  glGetProgramiv(program, pname, &success);
  if (success == 1) {
    return;
  }

  GLint length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  std::vector<char> info(length);

  glGetProgramInfoLog(program, length, nullptr, info.data());
  std::println(stderr, "Error:\n{}", std::string_view(info.data(), info.size()));
}

auto main() -> int {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, NAME, nullptr, nullptr);
  if (window == nullptr) {
    std::println(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, onFramebufferSize);

  if (gladLoadGL((GLADloadfunc)glfwGetProcAddress) == 0) {
    std::println(stderr, "Failed to initialize GLAD");
    return EXIT_FAILURE;
  }

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  checkShaderStatus(vertexShader, GL_COMPILE_STATUS);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  checkShaderStatus(fragmentShader, GL_COMPILE_STATUS);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  checkProgramStatus(shaderProgram, GL_LINK_STATUS);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  std::array vertices = {
      // clang-format off
      // T0
      +0.25F, +0.25F, 0.0F,
      +0.50F, +0.25F, 0.0F,
      +0.50F, +0.50F, 0.0F,
      // T1
      -0.50F, +0.25F, 0.0F,
      -0.25F, +0.25F, 0.0F,
      -0.50F, +0.50F, 0.0F,
      // clang-format on
  };
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

  GLuint vertexBufferObj = 0;
  GLuint vertexArrayObj = 0;
  glGenVertexArrays(1, &vertexArrayObj);
  glGenBuffers(1, &vertexBufferObj);

  glBindVertexArray(vertexArrayObj);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(vertices[0]),
               vertices.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(vertices[0]), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (glfwWindowShouldClose(window) == 0) {
    processInput(window);

    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArrayObj);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertexArrayObj);
  glDeleteBuffers(1, &vertexBufferObj);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

void onFramebufferSize(GLFWwindow * /*unused*/, int width, int height) {
  glViewport(0, 0, width, height);
}
