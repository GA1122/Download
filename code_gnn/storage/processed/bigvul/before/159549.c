void Unpack<WebGLImageConversion::kDataFormatARGB8, uint8_t, float>(
    const uint8_t* source,
    float* destination,
    unsigned pixels_per_row) {
  const float kScaleFactor = 1.0f / 255.0f;
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    destination[0] = source[1] * kScaleFactor;
    destination[1] = source[2] * kScaleFactor;
    destination[2] = source[3] * kScaleFactor;
    destination[3] = source[0] * kScaleFactor;
    source += 4;
    destination += 4;
  }
}
