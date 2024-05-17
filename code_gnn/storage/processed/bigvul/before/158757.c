ScopedTextureBinder::~ScopedTextureBinder() {
  ScopedGLErrorSuppressor suppressor(
      "ScopedTextureBinder::dtor", state_->GetErrorState());
  RestoreCurrentTextureBindings(state_, target_, 0);
  state_->RestoreActiveTexture();
}
