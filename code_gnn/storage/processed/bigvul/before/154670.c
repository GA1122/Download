error::Error GLES2DecoderPassthroughImpl::DoGenVertexArraysOES(
    GLsizei n,
    volatile GLuint* arrays) {
  return GenHelper(n, arrays, &vertex_array_id_map_,
                   [this](GLsizei n, GLuint* arrays) {
                     api()->glGenVertexArraysOESFn(n, arrays);
                   });
}
