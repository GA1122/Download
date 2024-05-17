error::Error GLES2DecoderImpl::GetFragDataLocationHelper(
    GLuint client_id,
    uint32_t location_shm_id,
    uint32_t location_shm_offset,
    const std::string& name_str) {
  const char kFunctionName[] = "glGetFragDataLocation";
  GLint* location = GetSharedMemoryAs<GLint*>(
      location_shm_id, location_shm_offset, sizeof(GLint));
  if (!location) {
    return error::kOutOfBounds;
  }
  if (*location != -1) {
    return error::kInvalidArguments;
  }
  Program* program = GetProgramInfoNotShader(client_id, kFunctionName);
  if (!program) {
    return error::kNoError;
  }
  if (!program->IsValid()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                       "program not linked");
    return error::kNoError;
  }

  *location = program->GetFragDataLocation(name_str);
  return error::kNoError;
}
