bool DoImagesMatch(const gfx::Image& a, const gfx::Image& b) {
  SkBitmap a_bitmap = a.AsBitmap();
  SkBitmap b_bitmap = b.AsBitmap();

  if (a_bitmap.width() != b_bitmap.width() ||
      a_bitmap.height() != b_bitmap.height()) {
    return false;
  }
  return memcmp(a_bitmap.getPixels(), b_bitmap.getPixels(),
                a_bitmap.computeByteSize()) == 0;
}
