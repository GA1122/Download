AcceleratedStaticBitmapImage::ContextProviderWrapper() const {
  if (!IsValid())
    return nullptr;
  return texture_holder_->ContextProviderWrapper();
 }
