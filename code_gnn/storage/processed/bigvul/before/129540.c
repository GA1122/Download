ScopedResolvedFrameBufferBinder::~ScopedResolvedFrameBufferBinder() {
  if (!resolve_and_bind_)
    return;

  ScopedGLErrorSuppressor suppressor(
      "ScopedResolvedFrameBufferBinder::dtor", decoder_->GetErrorState());
  decoder_->RestoreCurrentFramebufferBindings();
  if (decoder_->state_.enable_flags.scissor_test) {
    decoder_->state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, true);
  }
}
