void Pack<WebGLImageConversion::kDataFormatRGB32F,
          WebGLImageConversion::kAlphaDoPremultiply,
          float,
          float>(const float* source,
                 float* destination,
                 unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = source[3];
    destination[0] = source[0] * scale_factor;
    destination[1] = source[1] * scale_factor;
    destination[2] = source[2] * scale_factor;
    source += 4;
    destination += 3;
  }
}