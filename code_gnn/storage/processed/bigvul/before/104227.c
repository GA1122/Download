  const int8* GetPixelAddress(const int8* base, GLint x, GLint y) const {
    return base + (width_ * y + x) * bytes_per_pixel_;
  }
