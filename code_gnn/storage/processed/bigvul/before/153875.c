void BackFramebuffer::AttachRenderTexture(BackTexture* texture) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor("BackFramebuffer::AttachRenderTexture",
                                     decoder_->error_state_.get());
  ScopedFramebufferBinder binder(decoder_, id_);
  GLuint attach_id = texture ? texture->id() : 0;
  api()->glFramebufferTexture2DEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                     texture->Target(), attach_id, 0);
}
