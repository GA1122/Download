error::Error GLES2DecoderPassthroughImpl::DoGetTransformFeedbackVarying(
    GLuint program,
    GLuint index,
    GLsizei* size,
    GLenum* type,
    std::string* name,
    int32_t* success) {
  CheckErrorCallbackState();

  GLuint service_id = GetProgramServiceID(program, resources_);
  GLint transform_feedback_varying_max_length = 0;
  api()->glGetProgramivFn(service_id, GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,
                          &transform_feedback_varying_max_length);
  if (CheckErrorCallbackState()) {
    *success = 0;
    return error::kNoError;
  }

  std::vector<char> name_buffer(transform_feedback_varying_max_length, 0);
  api()->glGetTransformFeedbackVaryingFn(service_id, index, name_buffer.size(),
                                         nullptr, size, type,
                                         name_buffer.data());
  *name = std::string(name_buffer.data());
  *success = CheckErrorCallbackState() ? 0 : 1;
  return error::kNoError;
}
