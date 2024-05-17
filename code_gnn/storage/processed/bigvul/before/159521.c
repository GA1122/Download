void Pack<WebGLImageConversion::kDataFormatRGBA32_S,
          WebGLImageConversion::kAlphaDoPremultiply,
          int32_t,
          int32_t>(const int32_t* source,
                   int32_t* destination,
                   unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    destination[3] = ClampMin(source[3]);
    double scale_factor = static_cast<double>(destination[3]) / kMaxInt32Value;
    destination[0] = static_cast<int32_t>(
        static_cast<double>(ClampMin(source[0])) * scale_factor);
    destination[1] = static_cast<int32_t>(
        static_cast<double>(ClampMin(source[1])) * scale_factor);
    destination[2] = static_cast<int32_t>(
        static_cast<double>(ClampMin(source[2])) * scale_factor);
    source += 4;
    destination += 4;
  }
}
