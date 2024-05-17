std::string EncodeSkBitmap(const SkBitmap& image,
                           const std::string& format,
                           int quality) {
  return EncodeImage(gfx::Image::CreateFrom1xBitmap(image), format, quality);
}
