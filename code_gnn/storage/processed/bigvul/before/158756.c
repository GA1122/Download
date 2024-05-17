ScopedFramebufferBinder::~ScopedFramebufferBinder() {
  ScopedGLErrorSuppressor suppressor(
      "ScopedFramebufferBinder::dtor", decoder_->GetErrorState());
  decoder_->RestoreCurrentFramebufferBindings();
}
