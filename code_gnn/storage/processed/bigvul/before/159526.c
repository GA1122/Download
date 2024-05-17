void Pack<WebGLImageConversion::kDataFormatRG8,
          WebGLImageConversion::kAlphaDoPremultiply,
          uint8_t,
          uint8_t>(const uint8_t* source,
                   uint8_t* destination,
                   unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = static_cast<float>(source[3]) / kMaxUInt8Value;
    destination[0] =
        static_cast<uint8_t>(static_cast<float>(source[0]) * scale_factor);
    destination[1] =
        static_cast<uint8_t>(static_cast<float>(source[1]) * scale_factor);
    source += 4;
    destination += 2;
  }
}
