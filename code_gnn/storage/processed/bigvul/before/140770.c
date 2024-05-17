error::Error GLES2DecoderImpl::GetFragDataLocationHelper(
    GLuint client_id, uint32 location_shm_id, uint32 location_shm_offset,
    const std::string& name_str) {
  GLint* location = GetSharedMemoryAs<GLint*>(
      location_shm_id, location_shm_offset, sizeof(GLint));
  if (!location) {
    return error::kOutOfBounds;
  }
  if (*location != -1) {
    return error::kGenericError;
  }
  Program* program = GetProgramInfoNotShader(
      client_id, "glGetFragDataLocation");
  if (!program) {
    return error::kNoError;
  }
  *location = glGetFragDataLocation(program->service_id(), name_str.c_str());
  return error::kNoError;
}
