void GLES2DecoderImpl::DoDetachShader(
    GLuint program_client_id, GLint shader_client_id) {
  ProgramManager::ProgramInfo* program_info = GetProgramInfoNotShader(
      program_client_id, "glDetachShader");
  if (!program_info) {
    return;
  }
  ShaderManager::ShaderInfo* shader_info = GetShaderInfoNotProgram(
      shader_client_id, "glDetachShader");
  if (!shader_info) {
    return;
  }
  if (!program_info->DetachShader(shader_manager(), shader_info)) {
    SetGLError(GL_INVALID_OPERATION,
               "glDetachShader: shader not attached to program");
    return;
  }
  glDetachShader(program_info->service_id(), shader_info->service_id());
}
