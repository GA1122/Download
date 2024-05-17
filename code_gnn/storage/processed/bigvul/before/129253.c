void GLES2DecoderImpl::DoBindVertexArrayOES(GLuint client_id) {
  VertexAttribManager* vao = NULL;
  if (client_id != 0) {
    vao = GetVertexAttribManager(client_id);
    if (!vao) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION,
          "glBindVertexArrayOES", "bad vertex array id.");
      current_decoder_error_ = error::kNoError;
      return;
    }
  } else {
    vao = state_.default_vertex_attrib_manager.get();
  }

  if (state_.vertex_attrib_manager.get() != vao) {
    state_.vertex_attrib_manager = vao;
    if (!features().native_vertex_array_object) {
      EmulateVertexArrayState();
    } else {
      GLuint service_id = vao->service_id();
      glBindVertexArrayOES(service_id);
    }
  }
}
