#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <filesystem>
#include <glad/gl.h>

class Program {
public:
  Program(const std::filesystem::path &vertexShaderPath,
          const std::filesystem::path &fragmentShaderPath);
  void use() const;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

private:
  GLuint programId;
};

#endif // PROGRAM_H_
