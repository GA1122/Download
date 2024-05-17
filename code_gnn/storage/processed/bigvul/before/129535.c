ScopedFrameBufferBinder::~ScopedFrameBufferBinder() {
  ScopedGLErrorSuppressor suppressor(
      "ScopedFrameBufferBinder::dtor", decoder_->GetErrorState());
  decoder_->RestoreCurrentFramebufferBindings();
}
