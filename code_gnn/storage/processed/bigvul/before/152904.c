ImageBitmap* ImageBitmap::take(ScriptPromiseResolver*, sk_sp<SkImage> image) {
  return ImageBitmap::create(StaticBitmapImage::create(std::move(image)));
}
