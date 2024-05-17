UnacceleratedStaticBitmapImage::Create(sk_sp<SkImage> image) {
  DCHECK(!image->isTextureBacked());
  return base::AdoptRef(new UnacceleratedStaticBitmapImage(std::move(image)));
}
