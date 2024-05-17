void GLES2DecoderImpl::DeleteVertexArraysOESHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    VertexAttribManager* vao =
        GetVertexAttribManager(client_ids[ii]);
    if (vao && !vao->IsDeleted()) {
      if (state_.vertex_attrib_manager.get() == vao) {
        DoBindVertexArrayOES(0);
      }
      RemoveVertexAttribManager(client_ids[ii]);
    }
  }
}
