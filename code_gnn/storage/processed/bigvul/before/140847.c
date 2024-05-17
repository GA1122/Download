bool GLES2DecoderImpl::PrepForSetUniformByLocation(
    GLint fake_location,
    const char* function_name,
    Program::UniformApiType api_type,
    GLint* real_location,
    GLenum* type,
    GLsizei* count) {
  DCHECK(type);
  DCHECK(count);
  DCHECK(real_location);

  if (!CheckCurrentProgramForUniform(fake_location, function_name)) {
    return false;
  }
  GLint array_index = -1;
  const Program::UniformInfo* info =
      state_.current_program->GetUniformInfoByFakeLocation(
          fake_location, real_location, &array_index);
  if (!info) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "unknown location");
    return false;
  }
  if (!CheckUniformForApiType(info, function_name, api_type)) {
    return false;
  }
  if (*count > 1 && !info->is_array) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "count > 1 for non-array");
    return false;
  }
  *count = std::min(info->size - array_index, *count);
  if (*count <= 0) {
    return false;
  }
  *type = info->type;
  return true;
}
