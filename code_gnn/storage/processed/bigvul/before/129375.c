  Program* GetProgramInfoNotShader(
      GLuint client_id, const char* function_name) {
    Program* program = GetProgram(client_id);
    if (!program) {
      if (GetShader(client_id)) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name, "shader passed for program");
      } else {
        LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "unknown program");
      }
    }
    LogClientServiceForInfo(program, client_id, function_name);
    return program;
  }
