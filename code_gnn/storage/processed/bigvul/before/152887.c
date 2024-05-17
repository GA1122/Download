ImageBitmap* ImageBitmap::create(HTMLCanvasElement* canvas,
                                 Optional<IntRect> cropRect,
                                 const ImageBitmapOptions& options) {
  return new ImageBitmap(canvas, cropRect, options);
}
