void DXVAVideoDecodeAccelerator::NotifyPictureReady(
    const media::Picture& picture) {
  if (state_ != kUninitialized && client_)
    client_->PictureReady(picture);
}
