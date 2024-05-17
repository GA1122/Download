void BackFramebuffer::AttachRenderTexture(BackTexture* texture) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor(
      "BackFramebuffer::AttachRenderTexture", decoder_->GetErrorState());
  ScopedFramebufferBinder binder(decoder_, id_);
  GLuint attach_id = texture ? texture->id() : 0;
  api()->glFramebufferTexture2DEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                     texture->Target(), attach_id, 0);
}
