void BackFramebuffer::Create() {
  ScopedGLErrorSuppressor suppressor("BackFramebuffer::Create",
                                     decoder_->error_state_.get());
  Destroy();
  api()->glGenFramebuffersEXTFn(1, &id_);
}
