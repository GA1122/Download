ScopedRenderBufferBinder::~ScopedRenderBufferBinder() {
  ScopedGLErrorSuppressor suppressor("ScopedRenderBufferBinder::dtor",
                                     error_state_);
  state_->RestoreRenderbufferBindings();
}
