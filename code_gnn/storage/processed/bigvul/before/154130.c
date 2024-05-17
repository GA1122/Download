bool GLES2DecoderImpl::GenVertexArraysOESHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (GetVertexAttribManager(client_ids[ii])) {
      return false;
    }
  }

  if (!features().native_vertex_array_object) {
    for (GLsizei ii = 0; ii < n; ++ii) {
      CreateVertexAttribManager(client_ids[ii], 0, true);
    }
  } else {
    std::unique_ptr<GLuint[]> service_ids(new GLuint[n]);

    api()->glGenVertexArraysOESFn(n, service_ids.get());
    for (GLsizei ii = 0; ii < n; ++ii) {
      CreateVertexAttribManager(client_ids[ii], service_ids[ii], true);
    }
  }

  return true;
}
