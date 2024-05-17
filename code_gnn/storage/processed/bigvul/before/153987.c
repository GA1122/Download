void GLES2DecoderImpl::DoDetachShader(
    GLuint program_client_id, GLint shader_client_id) {
  Program* program = GetProgramInfoNotShader(
      program_client_id, "glDetachShader");
  if (!program) {
    return;
  }
  Shader* shader = GetShaderInfoNotProgram(shader_client_id, "glDetachShader");
  if (!shader) {
    return;
  }
  if (!program->IsShaderAttached(shader)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glDetachShader", "shader not attached to program");
    return;
  }
  api()->glDetachShaderFn(program->service_id(), shader->service_id());
  program->DetachShader(shader_manager(), shader);
}
