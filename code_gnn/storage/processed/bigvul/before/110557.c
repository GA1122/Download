  ShaderManager::ShaderInfo* GetShaderInfoNotProgram(
      GLuint client_id, const char* function_name) {
    ShaderManager::ShaderInfo* info = GetShaderInfo(client_id);
    if (!info) {
      if (GetProgramInfo(client_id)) {
        SetGLError(
            GL_INVALID_OPERATION, function_name, "program passed for shader");
      } else {
        SetGLError(
            GL_INVALID_VALUE, function_name, "unknown shader");
      }
    }
    return info;
  }
