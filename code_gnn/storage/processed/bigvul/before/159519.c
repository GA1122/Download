void Pack<WebGLImageConversion::kDataFormatRGBA16_S,
          WebGLImageConversion::kAlphaDoPremultiply,
          int16_t,
          int16_t>(const int16_t* source,
                   int16_t* destination,
                   unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    destination[3] = ClampMin(source[3]);
    float scale_factor = static_cast<float>(destination[3]) / kMaxInt16Value;
    destination[0] = static_cast<int16_t>(
        static_cast<float>(ClampMin(source[0])) * scale_factor);
    destination[1] = static_cast<int16_t>(
        static_cast<float>(ClampMin(source[1])) * scale_factor);
    destination[2] = static_cast<int16_t>(
        static_cast<float>(ClampMin(source[2])) * scale_factor);
    source += 4;
    destination += 4;
  }
}
