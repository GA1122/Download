ScopedTexture2DBindingReset::~ScopedTexture2DBindingReset() {
  api_->glBindTextureFn(GL_TEXTURE_2D, texture_);
}
