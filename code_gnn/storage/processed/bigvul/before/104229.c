  ReadPixelsEmulator(GLsizei width, GLsizei height, GLint bytes_per_pixel,
                     const void* src_pixels, const void* expected_pixels,
                     GLint pack_alignment)
      : width_(width),
        height_(height),
        pack_alignment_(pack_alignment),
        bytes_per_pixel_(bytes_per_pixel),
        src_pixels_(reinterpret_cast<const int8*>(src_pixels)),
        expected_pixels_(reinterpret_cast<const int8*>(expected_pixels)) {
  }
