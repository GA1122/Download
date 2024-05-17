  ProgramManager::ProgramInfo* GetProgramInfoNotShader(
      GLuint client_id, const char* function_name) {
    ProgramManager::ProgramInfo* info = GetProgramInfo(client_id);
    if (!info) {
      if (GetShaderInfo(client_id)) {
        SetGLError(
            GL_INVALID_OPERATION, function_name, "shader passed for program");
      } else {
        SetGLError(GL_INVALID_VALUE, function_name, "unknown program");
      }
    }
    return info;
  }
