void Pack<WebGLImageConversion::kDataFormatRGBA16,
          WebGLImageConversion::kAlphaDoPremultiply,
          uint16_t,
          uint16_t>(const uint16_t* source,
                    uint16_t* destination,
                    unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = static_cast<float>(source[3]) / kMaxUInt16Value;
    destination[0] =
        static_cast<uint16_t>(static_cast<float>(source[0]) * scale_factor);
    destination[1] =
        static_cast<uint16_t>(static_cast<float>(source[1]) * scale_factor);
    destination[2] =
        static_cast<uint16_t>(static_cast<float>(source[2]) * scale_factor);
    destination[3] = source[3];
    source += 4;
    destination += 4;
  }
}
