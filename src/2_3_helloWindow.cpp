#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <print>

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

  while (glfwWindowShouldClose(window) == 0) {
    processInput(window);

    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

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
