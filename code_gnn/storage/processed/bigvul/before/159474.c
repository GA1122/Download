void Pack<WebGLImageConversion::kDataFormatR8,
          WebGLImageConversion::kAlphaDoNothing,
          uint8_t,
          uint8_t>(const uint8_t* source,
                   uint8_t* destination,
                   unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    destination[0] = source[0];
    source += 4;
    destination += 1;
  }
}
