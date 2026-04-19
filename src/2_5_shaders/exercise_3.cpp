#include "../common/program.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <print>

#define SHADERS_SRC "src/2_5_shaders"

void onFramebufferSize(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

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

  auto program = Program(SHADERS_SRC "/exercise_3.vert", SHADERS_SRC "/exercise_3.frag");

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  std::array vertices = {
      // clang-format off
       0.5F, -0.5F, 0.0F,
      -0.5F, -0.5F, 0.0F,
       0.0F,  0.5F, 0.0F,
      // clang-format on
  };
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

  GLuint vertexArrayObj = 0;
  glGenVertexArrays(1, &vertexArrayObj);

  GLuint vertexBufferObj = 0;
  glGenBuffers(1, &vertexBufferObj);

  glBindVertexArray(vertexArrayObj);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof vertices[0],
               vertices.data(),
               GL_STATIC_DRAW);

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof vertices[0], nullptr);
  glEnableVertexAttribArray(0);

  while (glfwWindowShouldClose(window) == 0) {
    processInput(window);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    program.use();

    glBindVertexArray(vertexArrayObj);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertexArrayObj);
  glDeleteBuffers(1, &vertexBufferObj);

  return EXIT_SUCCESS;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

void onFramebufferSize(GLFWwindow * /*unused*/, int width, int height) {
  glViewport(0, 0, width, height);
}
