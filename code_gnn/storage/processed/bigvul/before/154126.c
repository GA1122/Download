bool GLES2DecoderImpl::GenRenderbuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (GetRenderbuffer(client_ids[ii])) {
      return false;
    }
  }
  std::unique_ptr<GLuint[]> service_ids(new GLuint[n]);
  api()->glGenRenderbuffersEXTFn(n, service_ids.get());
  for (GLsizei ii = 0; ii < n; ++ii) {
    CreateRenderbuffer(client_ids[ii], service_ids[ii]);
  }
  return true;
}
