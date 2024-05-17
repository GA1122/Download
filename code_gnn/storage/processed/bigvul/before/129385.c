error::Error GLES2DecoderImpl::GetUniformLocationHelper(
    GLuint client_id, uint32 location_shm_id, uint32 location_shm_offset,
    const std::string& name_str) {
  if (!StringIsValidForGLES(name_str.c_str())) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glGetUniformLocation", "Invalid character");
    return error::kNoError;
  }
  Program* program = GetProgramInfoNotShader(
      client_id, "glGetUniformLocation");
  if (!program) {
    return error::kNoError;
  }
  if (!program->IsValid()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glGetUniformLocation", "program not linked");
    return error::kNoError;
  }
  GLint* location = GetSharedMemoryAs<GLint*>(
      location_shm_id, location_shm_offset, sizeof(GLint));
  if (!location) {
    return error::kOutOfBounds;
  }
  if (*location != -1) {
    return error::kGenericError;
  }
  *location = program->GetUniformFakeLocation(name_str);
  return error::kNoError;
}
