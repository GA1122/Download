GLenum BackFramebuffer::CheckStatus() {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor("BackFramebuffer::CheckStatus",
                                     decoder_->GetErrorState());
  ScopedFramebufferBinder binder(decoder_, id_);
  return api()->glCheckFramebufferStatusEXTFn(GL_FRAMEBUFFER);
}
