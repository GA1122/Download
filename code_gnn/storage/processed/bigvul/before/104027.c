void GLES2DecoderImpl::DoGetProgramiv(
    GLuint program_id, GLenum pname, GLint* params) {
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program_id, "glGetProgramiv");
  if (!info) {
    return;
  }
  info->GetProgramiv(pname, params);
}
