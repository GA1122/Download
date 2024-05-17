ScopedResolvedFramebufferBinder::~ScopedResolvedFramebufferBinder() {
  if (!resolve_and_bind_)
    return;

  ScopedGLErrorSuppressor suppressor("ScopedResolvedFramebufferBinder::dtor",
                                     decoder_->error_state_.get());
  decoder_->RestoreCurrentFramebufferBindings();
  if (decoder_->state_.enable_flags.scissor_test) {
    decoder_->state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, true);
    decoder_->RestoreDeviceWindowRectangles();
  }
}
