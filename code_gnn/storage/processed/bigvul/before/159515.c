void Pack<WebGLImageConversion::kDataFormatR16F,
          WebGLImageConversion::kAlphaDoPremultiply,
          float,
          uint16_t>(const float* source,
                    uint16_t* destination,
                    unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = source[3];
    destination[0] = ConvertFloatToHalfFloat(source[0] * scale_factor);
    source += 4;
    destination += 1;
  }
}