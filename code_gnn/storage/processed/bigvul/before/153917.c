void BackRenderbuffer::Create() {
  ScopedGLErrorSuppressor suppressor("BackRenderbuffer::Create",
                                     decoder_->error_state_.get());
  Destroy();
  api()->glGenRenderbuffersEXTFn(1, &id_);
}
