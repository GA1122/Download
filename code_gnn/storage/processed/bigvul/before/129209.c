void BackFramebuffer::AttachRenderBuffer(GLenum target,
                                         BackRenderbuffer* render_buffer) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor(
      "BackFramebuffer::AttachRenderBuffer", decoder_->GetErrorState());
  ScopedFrameBufferBinder binder(decoder_, id_);
  GLuint attach_id = render_buffer ? render_buffer->id() : 0;
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER,
                               target,
                               GL_RENDERBUFFER,
                               attach_id);
}