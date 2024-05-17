ImageBitmap* ImageBitmap::create(PassRefPtr<StaticBitmapImage> image) {
  return new ImageBitmap(std::move(image));
}
