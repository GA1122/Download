void Pack<WebGLImageConversion::kDataFormatA32F,
          WebGLImageConversion::kAlphaDoNothing,
          float,
          float>(const float* source,
                 float* destination,
                 unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    destination[0] = source[3];
    source += 4;
    destination += 1;
  }
}
