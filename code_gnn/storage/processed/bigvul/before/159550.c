void Unpack<WebGLImageConversion::kDataFormatRA32F, float, float>(
    const float* source,
    float* destination,
    unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    destination[0] = source[0];
    destination[1] = source[0];
    destination[2] = source[0];
    destination[3] = source[1];
    source += 2;
    destination += 4;
  }
}
