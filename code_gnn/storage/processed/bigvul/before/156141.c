bool AcceleratedStaticBitmapImage::IsValid() const {
  return texture_holder_ && texture_holder_->IsValid();
}
