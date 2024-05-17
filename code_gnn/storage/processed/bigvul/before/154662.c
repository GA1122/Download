error::Error GLES2DecoderPassthroughImpl::DoGenBuffers(
    GLsizei n,
    volatile GLuint* buffers) {
  return GenHelper(n, buffers, &resources_->buffer_id_map,
                   [this](GLsizei n, GLuint* buffers) {
                     api()->glGenBuffersARBFn(n, buffers);
                   });
}
