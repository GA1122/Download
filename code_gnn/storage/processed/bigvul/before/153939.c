void BackFramebuffer::Destroy() {
  if (id_ != 0) {
    ScopedGLErrorSuppressor suppressor("BackFramebuffer::Destroy",
                                       decoder_->error_state_.get());
    api()->glDeleteFramebuffersEXTFn(1, &id_);
    id_ = 0;
  }
}
