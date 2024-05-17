void Pack<WebGLImageConversion::kDataFormatRGBA16F,
          WebGLImageConversion::kAlphaDoUnmultiply,
          float,
          uint16_t>(const float* source,
                    uint16_t* destination,
                    unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = source[3] ? 1.0f / source[3] : 1.0f;
    destination[0] = ConvertFloatToHalfFloat(source[0] * scale_factor);
    destination[1] = ConvertFloatToHalfFloat(source[1] * scale_factor);
    destination[2] = ConvertFloatToHalfFloat(source[2] * scale_factor);
    destination[3] = ConvertFloatToHalfFloat(source[3]);
    source += 4;
    destination += 4;
  }
}
