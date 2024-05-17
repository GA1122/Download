void DXVAVideoDecodeAccelerator::RequestPictureBuffers(int width, int height) {
  if (state_ != kUninitialized && client_) {
    client_->ProvidePictureBuffers(kNumPictureBuffers,
                                   gfx::Size(width, height));
  }
}
