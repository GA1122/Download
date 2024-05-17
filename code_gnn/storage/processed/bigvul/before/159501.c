void Pack<WebGLImageConversion::kDataFormatR32F,
          WebGLImageConversion::kAlphaDoPremultiply,
          float,
          float>(const float* source,
                 float* destination,
                 unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = source[3];
    destination[0] = source[0] * scale_factor;
    source += 4;
    destination += 1;
  }
}
