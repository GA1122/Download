ScopedRenderBufferBinder::~ScopedRenderBufferBinder() {
  ScopedGLErrorSuppressor suppressor(decoder_);
  decoder_->RestoreCurrentRenderbufferBindings();
}
