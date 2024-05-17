error::Error GLES2DecoderPassthroughImpl::DoDeleteVertexArraysOES(
    GLsizei n,
    const volatile GLuint* arrays) {
  return DeleteHelper(n, arrays, &vertex_array_id_map_,
                      [this](GLsizei n, GLuint* arrays) {
                        api()->glDeleteVertexArraysOESFn(n, arrays);
                      });
}
