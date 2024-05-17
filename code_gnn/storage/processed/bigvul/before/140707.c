bool GLES2DecoderImpl::CheckSubscriptionTarget(GLint location,
                                               GLenum subscription,
                                               const char* function_name) {
  if (!CheckCurrentProgramForUniform(location, function_name)) {
    return false;
  }
  GLint real_location = -1;
  GLint array_index = -1;
  const Program::UniformInfo* info =
      state_.current_program->GetUniformInfoByFakeLocation(
          location, &real_location, &array_index);
  if (!info) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name, "unknown location");
    return false;
  }
  if ((ValuebufferManager::ApiTypeForSubscriptionTarget(subscription) &
       info->accepts_api_type) == 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "wrong type for subscription");
    return false;
  }
  return true;
}
