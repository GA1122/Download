void Unpack<WebGLImageConversion::kDataFormatRGBA2_10_10_10, uint32_t, float>(
    const uint32_t* source,
    float* destination,
    unsigned pixels_per_row) {
  static const float kRgbScaleFactor = 1.0f / 1023.0f;
  static const float kAlphaScaleFactor = 1.0f / 3.0f;
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    uint32_t packed_value = source[0];
    destination[0] = static_cast<float>(packed_value & 0x3FF) * kRgbScaleFactor;
    destination[1] =
        static_cast<float>((packed_value >> 10) & 0x3FF) * kRgbScaleFactor;
    destination[2] =
        static_cast<float>((packed_value >> 20) & 0x3FF) * kRgbScaleFactor;
    destination[3] = static_cast<float>(packed_value >> 30) * kAlphaScaleFactor;
    source += 1;
    destination += 4;
  }
}
