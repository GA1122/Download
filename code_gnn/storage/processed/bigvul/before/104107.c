  ShaderManager::ShaderInfo* GetShaderInfoNotProgram(
      GLuint client_id, const char* function_name) {
    ShaderManager::ShaderInfo* info = GetShaderInfo(client_id);
    if (!info) {
      if (GetProgramInfo(client_id)) {
        SetGLError(
            GL_INVALID_OPERATION,
            (std::string(function_name) +
             ": program passed for shader").c_str());
      } else {
        SetGLError(GL_INVALID_VALUE,
                   (std::string(function_name) + ": unknown shader").c_str());
      }
    }
    return info;
  }
