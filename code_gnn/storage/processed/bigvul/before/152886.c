ImageBitmap* ImageBitmap::create(HTMLVideoElement* video,
                                 Optional<IntRect> cropRect,
                                 Document* document,
                                 const ImageBitmapOptions& options) {
  return new ImageBitmap(video, cropRect, document, options);
}
