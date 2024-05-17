ScopedRenderBufferBinder::~ScopedRenderBufferBinder() {
  ScopedGLErrorSuppressor suppressor(
      "ScopedRenderBufferBinder::dtor", state_->GetErrorState());
  state_->RestoreRenderbufferBindings();
}
