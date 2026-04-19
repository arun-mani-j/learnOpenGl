#include "program.hpp"
#include <fstream>
#include <print>
#include <sstream>
#include <stdexcept>
#include <vector>

static auto readFile(const std::filesystem::path &path) -> std::string {
  std::ifstream file(path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

static auto checkCompileStatus(GLuint shader) -> void {
  GLint success = 0;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == 1) {
    return;
  }

  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  std::vector<char> info(length);

  glGetShaderInfoLog(shader, length, nullptr, info.data());
  std::println(stderr, "Error:\n{}", std::string_view(info.data(), info.size()));

  throw std::runtime_error("failed to compile shader");
}

static auto checkLinkStatus(GLuint programId) -> void {
  GLint success = 0;

  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (success == 1) {
    return;
  }

  GLint length = 0;
  glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
  std::vector<char> info(length);

  glGetProgramInfoLog(programId, length, nullptr, info.data());
  std::println(stderr, "Error:\n{}", std::string_view(info.data(), info.size()));

  throw std::runtime_error("failed to link shaders");
}

Program::Program(const std::filesystem::path &vertexShaderPath,
                 const std::filesystem::path &fragmentShaderPath)
    : programId(glCreateProgram()) {
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  auto vertex_src = readFile(vertexShaderPath);
  const char *vertex_str = vertex_src.c_str();
  glShaderSource(vertexShaderId, 1, &vertex_str, nullptr);
  glCompileShader(vertexShaderId);
  checkCompileStatus(vertexShaderId);

  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  auto fragment_src = readFile(fragmentShaderPath);
  const char *fragment_str = fragment_src.c_str();
  glShaderSource(fragmentShaderId, 1, &fragment_str, nullptr);
  glCompileShader(fragmentShaderId);
  checkCompileStatus(fragmentShaderId);

  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);
  checkLinkStatus(programId);

  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);
}

void Program::use() const { glUseProgram(programId); }

void Program::setBool(const std::string &name, bool value) const {
  GLint location = glGetUniformLocation(programId, name.data());
  glUniform1i(location, static_cast<GLint>(value));
}

void Program::setInt(const std::string &name, int value) const {
  GLint location = glGetUniformLocation(programId, name.data());
  glUniform1i(location, value);
}

void Program::setFloat(const std::string &name, float value) const {
  GLint location = glGetUniformLocation(programId, name.data());
  glUniform1f(location, value);
}
