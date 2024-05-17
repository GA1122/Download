void Pack<WebGLImageConversion::kDataFormatRGB565,
          WebGLImageConversion::kAlphaDoNothing,
          uint8_t,
          uint16_t>(const uint8_t* source,
                    uint16_t* destination,
                    unsigned pixels_per_row) {
#if WTF_CPU_ARM_NEON
  SIMD::PackOneRowOfRGBA8ToUnsignedShort565(source, destination,
                                            pixels_per_row);
#endif
#if HAVE_MIPS_MSA_INTRINSICS
  SIMD::packOneRowOfRGBA8ToUnsignedShort565MSA(source, destination,
                                               pixels_per_row);
#endif
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    *destination = (((source[0] & 0xF8) << 8) | ((source[1] & 0xFC) << 3) |
                    ((source[2] & 0xF8) >> 3));
    source += 4;
    destination += 1;
  }
}