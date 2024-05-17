ScopedFramebufferBinder::~ScopedFramebufferBinder() {
  ScopedGLErrorSuppressor suppressor("ScopedFramebufferBinder::dtor",
                                     decoder_->error_state_.get());
  decoder_->RestoreCurrentFramebufferBindings();
}
