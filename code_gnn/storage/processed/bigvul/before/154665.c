error::Error GLES2DecoderPassthroughImpl::DoGenQueriesEXT(
    GLsizei n,
    volatile GLuint* queries) {
  return GenHelper(n, queries, &query_id_map_,
                   [this](GLsizei n, GLuint* queries) {
                     api()->glGenQueriesFn(n, queries);
                   });
}
