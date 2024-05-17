void Pack<WebGLImageConversion::kDataFormatRGBA4444,
          WebGLImageConversion::kAlphaDoNothing,
          uint8_t,
          uint16_t>(const uint8_t* source,
                    uint16_t* destination,
                    unsigned pixels_per_row) {
#if WTF_CPU_ARM_NEON
  SIMD::PackOneRowOfRGBA8ToUnsignedShort4444(source, destination,
                                             pixels_per_row);
#endif
#if HAVE_MIPS_MSA_INTRINSICS
  SIMD::packOneRowOfRGBA8ToUnsignedShort4444MSA(source, destination,
                                                pixels_per_row);
#endif
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    *destination = (((source[0] & 0xF0) << 8) | ((source[1] & 0xF0) << 4) |
                    (source[2] & 0xF0) | (source[3] >> 4));
    source += 4;
    destination += 1;
  }
}
