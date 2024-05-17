void BackTexture::DestroyNativeGpuMemoryBuffer(bool have_context) {
  if (image_) {
    ScopedGLErrorSuppressor suppressor(
        "BackTexture::DestroyNativeGpuMemoryBuffer",
        decoder_->state_.GetErrorState());

    image_->ReleaseTexImage(Target());

    decoder_->texture_manager()->SetLevelImage(texture_ref_.get(), Target(), 0,
                                               nullptr, Texture::UNBOUND);
    image_ = nullptr;
  }
}
