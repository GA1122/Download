bool GLES2DecoderImpl::GenBuffersHelper(GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (GetBuffer(client_ids[ii])) {
      return false;
    }
  }
  std::unique_ptr<GLuint[]> service_ids(new GLuint[n]);
  api()->glGenBuffersARBFn(n, service_ids.get());
  for (GLsizei ii = 0; ii < n; ++ii) {
    CreateBuffer(client_ids[ii], service_ids[ii]);
  }
  return true;
}