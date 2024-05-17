void GLES2DecoderImpl::DoAttachShader(
    GLuint program_client_id, GLint shader_client_id) {
  ProgramManager::ProgramInfo* program_info = GetProgramInfoNotShader(
      program_client_id, "glAttachShader");
  if (!program_info) {
    return;
  }
  ShaderManager::ShaderInfo* shader_info = GetShaderInfoNotProgram(
      shader_client_id, "glAttachShader");
  if (!shader_info) {
    return;
  }
  if (!program_info->AttachShader(shader_manager(), shader_info)) {
    SetGLError(GL_INVALID_OPERATION,
               "glAttachShader: can not attach more than"
               " one shader of the same type.");
    return;
  }
  glAttachShader(program_info->service_id(), shader_info->service_id());
}
