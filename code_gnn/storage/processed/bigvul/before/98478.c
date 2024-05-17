  int GetTabPixel(int x, int y) const {
    const int* tab_pixels = reinterpret_cast<int*>(tab_bitmap_.getPixels());
    if (!tab_pixels || x >= tab_bitmap_.width() || y >= tab_bitmap_.height())
      return 0xFFFFFFFF;

    return 0xFF000000 | tab_pixels[y * tab_bitmap_.width() + x];
  }
