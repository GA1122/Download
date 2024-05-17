void GLES2DecoderImpl::RestoreCurrentRenderbufferBindings() {
  RenderbufferManager::RenderbufferInfo* renderbuffer =
      GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
  glBindRenderbufferEXT(
      GL_RENDERBUFFER, renderbuffer ? renderbuffer->service_id() : 0);
}
