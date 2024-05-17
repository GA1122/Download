ImageBitmap* ImageBitmap::create(HTMLImageElement* image,
                                 Optional<IntRect> cropRect,
                                 Document* document,
                                 const ImageBitmapOptions& options) {
  return new ImageBitmap(image, cropRect, document, options);
}
