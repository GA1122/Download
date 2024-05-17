void BackTexture::DestroyNativeGpuMemoryBuffer(bool have_context) {
  if (image_) {
    ScopedGLErrorSuppressor suppressor(
        "BackTexture::DestroyNativeGpuMemoryBuffer",
        decoder_->error_state_.get());

    image_->ReleaseTexImage(Target());

    decoder_->texture_manager()->SetLevelImage(texture_ref_.get(), Target(), 0,
                                               nullptr, Texture::UNBOUND);
    image_ = nullptr;
  }
}
