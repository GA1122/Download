void GLES2DecoderImpl::DoBindUniformLocationCHROMIUM(
    GLuint program, GLint location, const char* name) {
  if (!StringIsValidForGLES(name)) {
    SetGLError(GL_INVALID_VALUE,
               "glBindUniformLocationCHROMIUM", "Invalid character");
    return;
  }
  if (ProgramManager::IsInvalidPrefix(name, strlen(name))) {
    SetGLError(GL_INVALID_OPERATION,
               "glBindUniformLocationCHROMIUM", "reserved prefix");
    return;
  }
  if (location < 0 || static_cast<uint32>(location) >=
      (group_->max_fragment_uniform_vectors() +
       group_->max_vertex_uniform_vectors()) * 4) {
    SetGLError(GL_INVALID_VALUE,
               "glBindUniformLocationCHROMIUM", "location out of range");
    return;
  }
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program, "glBindUniformLocationCHROMIUM");
  if (!info) {
    return;
  }
  if (!info->SetUniformLocationBinding(name, location)) {
    SetGLError(GL_INVALID_VALUE,
               "glBindUniformLocationCHROMIUM", "location out of range");
  }
}
