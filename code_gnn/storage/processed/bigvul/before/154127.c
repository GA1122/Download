bool GLES2DecoderImpl::GenSamplersHelper(GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (GetSampler(client_ids[ii])) {
      return false;
    }
  }
  std::unique_ptr<GLuint[]> service_ids(new GLuint[n]);
  api()->glGenSamplersFn(n, service_ids.get());
  for (GLsizei ii = 0; ii < n; ++ii) {
    CreateSampler(client_ids[ii], service_ids[ii]);
  }
  return true;
}
