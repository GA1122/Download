void GLES2DecoderImpl::DoBindRenderbuffer(GLenum target, GLuint client_id) {
  Renderbuffer* renderbuffer = NULL;
  GLuint service_id = 0;
  if (client_id != 0) {
    renderbuffer = GetRenderbuffer(client_id);
    if (!renderbuffer) {
      if (!group_->bind_generates_resource()) {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                           "glBindRenderbuffer",
                           "id not generated by glGenRenderbuffers");
        return;
      }

      glGenRenderbuffersEXT(1, &service_id);
      CreateRenderbuffer(client_id, service_id);
      renderbuffer = GetRenderbuffer(client_id);
    } else {
      service_id = renderbuffer->service_id();
    }
    renderbuffer->MarkAsValid();
  }
  LogClientServiceForInfo(renderbuffer, client_id, "glBindRenderbuffer");
  state_.bound_renderbuffer = renderbuffer;
  state_.bound_renderbuffer_valid = true;
  glBindRenderbufferEXT(GL_RENDERBUFFER, service_id);
}