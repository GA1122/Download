ScopedTextureBinder::~ScopedTextureBinder() {
  ScopedGLErrorSuppressor suppressor("ScopedTextureBinder::dtor", error_state_);
  RestoreCurrentTextureBindings(state_, target_, 0);
  state_->RestoreActiveTexture();
}
