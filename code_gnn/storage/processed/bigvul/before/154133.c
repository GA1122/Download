error::Error GLES2DecoderImpl::GetAttribLocationHelper(
    GLuint client_id,
    uint32_t location_shm_id,
    uint32_t location_shm_offset,
    const std::string& name_str) {
  if (!StringIsValidForGLES(name_str)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glGetAttribLocation", "Invalid character");
    return error::kNoError;
  }
  Program* program = GetProgramInfoNotShader(
      client_id, "glGetAttribLocation");
  if (!program) {
    return error::kNoError;
  }
  if (!program->IsValid()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glGetAttribLocation", "program not linked");
    return error::kNoError;
  }
  GLint* location = GetSharedMemoryAs<GLint*>(
      location_shm_id, location_shm_offset, sizeof(GLint));
  if (!location) {
    return error::kOutOfBounds;
  }
  if (*location != -1) {
    return error::kInvalidArguments;
  }
  *location = program->GetAttribLocation(name_str);
  return error::kNoError;
}
