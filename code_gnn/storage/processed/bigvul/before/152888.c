ImageBitmap* ImageBitmap::create(ImageData* data,
                                 Optional<IntRect> cropRect,
                                 const ImageBitmapOptions& options) {
  return new ImageBitmap(data, cropRect, options);
}
