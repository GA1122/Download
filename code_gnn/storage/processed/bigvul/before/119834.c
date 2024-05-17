bool DoImagesMatch(const gfx::Image& a, const gfx::Image& b) {
  SkBitmap a_bitmap = a.AsBitmap();
  SkBitmap b_bitmap = b.AsBitmap();

  if (a_bitmap.width() != b_bitmap.width() ||
      a_bitmap.height() != b_bitmap.height()) {
    return false;
  }
  SkAutoLockPixels a_bitmap_lock(a_bitmap);
  SkAutoLockPixels b_bitmap_lock(b_bitmap);
  return memcmp(a_bitmap.getPixels(),
                b_bitmap.getPixels(),
                a_bitmap.getSize()) == 0;
}
