ScopedFrameBufferBinder::~ScopedFrameBufferBinder() {
  ScopedGLErrorSuppressor suppressor(decoder_);
  decoder_->RestoreCurrentFramebufferBindings();
}
