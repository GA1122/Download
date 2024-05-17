void Pack<WebGLImageConversion::kDataFormatRGBA2_10_10_10,
          WebGLImageConversion::kAlphaDoUnmultiply,
          float,
          uint32_t>(const float* source,
                    uint32_t* destination,
                    unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = source[3] ? 1023.0f / source[3] : 1023.0f;
    uint32_t r = static_cast<uint32_t>(source[0] * scale_factor);
    uint32_t g = static_cast<uint32_t>(source[1] * scale_factor);
    uint32_t b = static_cast<uint32_t>(source[2] * scale_factor);
    uint32_t a = static_cast<uint32_t>(source[3] * 3.0f);
    destination[0] = (a << 30) | (b << 20) | (g << 10) | r;
    source += 4;
    destination += 1;
  }
}
