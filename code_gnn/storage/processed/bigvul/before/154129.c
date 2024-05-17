bool GLES2DecoderImpl::GenTransformFeedbacksHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (GetTransformFeedback(client_ids[ii])) {
      return false;
    }
  }
  std::unique_ptr<GLuint[]> service_ids(new GLuint[n]);
  api()->glGenTransformFeedbacksFn(n, service_ids.get());
  for (GLsizei ii = 0; ii < n; ++ii) {
    CreateTransformFeedback(client_ids[ii], service_ids[ii]);
  }
  return true;
}
