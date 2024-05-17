WebGraphicsContext3DProvider* AcceleratedStaticBitmapImage::ContextProvider()
    const {
  if (!IsValid())
    return nullptr;
  return texture_holder_->ContextProvider();
}
