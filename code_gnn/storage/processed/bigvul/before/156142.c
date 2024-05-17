AcceleratedStaticBitmapImage::MakeUnaccelerated() {
  CreateImageFromMailboxIfNeeded();
  return StaticBitmapImage::Create(
      texture_holder_->GetSkImage()->makeNonTextureImage());
}
