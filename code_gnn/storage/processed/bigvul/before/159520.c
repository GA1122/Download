void Pack<WebGLImageConversion::kDataFormatRGBA32,
          WebGLImageConversion::kAlphaDoPremultiply,
          uint32_t,
          uint32_t>(const uint32_t* source,
                    uint32_t* destination,
                    unsigned pixels_per_row) {
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    double scale_factor = static_cast<double>(source[3]) / kMaxUInt32Value;
    destination[0] =
        static_cast<uint32_t>(static_cast<double>(source[0]) * scale_factor);
    destination[1] =
        static_cast<uint32_t>(static_cast<double>(source[1]) * scale_factor);
    destination[2] =
        static_cast<uint32_t>(static_cast<double>(source[2]) * scale_factor);
    destination[3] = source[3];
    source += 4;
    destination += 4;
  }
}
