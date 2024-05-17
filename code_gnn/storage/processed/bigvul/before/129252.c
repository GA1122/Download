void GLES2DecoderImpl::DoBindUniformLocationCHROMIUM(
    GLuint program_id, GLint location, const char* name) {
  if (!StringIsValidForGLES(name)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        "glBindUniformLocationCHROMIUM", "Invalid character");
    return;
  }
  if (ProgramManager::IsInvalidPrefix(name, strlen(name))) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glBindUniformLocationCHROMIUM", "reserved prefix");
    return;
  }
  if (location < 0 || static_cast<uint32>(location) >=
      (group_->max_fragment_uniform_vectors() +
       group_->max_vertex_uniform_vectors()) * 4) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        "glBindUniformLocationCHROMIUM", "location out of range");
    return;
  }
  Program* program = GetProgramInfoNotShader(
      program_id, "glBindUniformLocationCHROMIUM");
  if (!program) {
    return;
  }
  if (!program->SetUniformLocationBinding(name, location)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        "glBindUniformLocationCHROMIUM", "location out of range");
  }
}
