error::Error GLES2DecoderPassthroughImpl::DoGenFramebuffers(
    GLsizei n,
    volatile GLuint* framebuffers) {
  return GenHelper(n, framebuffers, &framebuffer_id_map_,
                   [this](GLsizei n, GLuint* framebuffers) {
                     api()->glGenFramebuffersEXTFn(n, framebuffers);
                   });
}
