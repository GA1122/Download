void BackTexture::Destroy() {
  if (image_) {
    DCHECK(texture_ref_);
    ScopedTextureBinder binder(&decoder_->state_, decoder_->error_state_.get(),
                               id(), Target());
    DestroyNativeGpuMemoryBuffer(true);
  }

  if (texture_ref_) {
    ScopedGLErrorSuppressor suppressor("BackTexture::Destroy",
                                       decoder_->error_state_.get());
    texture_ref_ = nullptr;
  }
  memory_tracker_.TrackMemFree(bytes_allocated_);
  bytes_allocated_ = 0;
}
