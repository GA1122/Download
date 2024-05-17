UnacceleratedStaticBitmapImage::Create(PaintImage image) {
  return base::AdoptRef(new UnacceleratedStaticBitmapImage(std::move(image)));
}
