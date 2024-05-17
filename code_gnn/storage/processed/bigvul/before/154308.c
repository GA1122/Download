void BackTexture::Invalidate() {
  if (image_) {
    DestroyNativeGpuMemoryBuffer(false);
    image_ = nullptr;
  }
  if (texture_ref_) {
    texture_ref_->ForceContextLost();
    texture_ref_ = nullptr;
  }
}
