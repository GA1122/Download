ImageBitmap* ImageBitmap::create(ImageBitmap* bitmap,
                                 Optional<IntRect> cropRect,
                                 const ImageBitmapOptions& options) {
  return new ImageBitmap(bitmap, cropRect, options);
}
