ImageBitmap* ImageBitmap::create(PassRefPtr<StaticBitmapImage> image,
                                 Optional<IntRect> cropRect,
                                 const ImageBitmapOptions& options) {
  return new ImageBitmap(std::move(image), cropRect, options);
}
