  int GetPixel(const SkBitmap& bitmap, int x, int y) const {
    const int* tab_pixels = reinterpret_cast<const int*>(bitmap.getPixels());
    if (!tab_pixels)
      return 0xFFFFFFFF;
    return tab_pixels[y * bitmap.width() + x];
  }
