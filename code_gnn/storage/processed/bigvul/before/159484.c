void Pack<WebGLImageConversion::kDataFormatRGBA8,
          WebGLImageConversion::kAlphaDoUnmultiply,
          uint8_t,
          uint8_t>(const uint8_t* source,
                   uint8_t* destination,
                   unsigned pixels_per_row) {
#if defined(ARCH_CPU_X86_FAMILY)
  SIMD::PackOneRowOfRGBA8LittleToRGBA8(source, destination, pixels_per_row);
#endif
#if HAVE_MIPS_MSA_INTRINSICS
  SIMD::packOneRowOfRGBA8LittleToRGBA8MSA(source, destination, pixels_per_row);
#endif
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    float scale_factor = source[3] ? 255.0f / source[3] : 1.0f;
    uint8_t source_r =
        static_cast<uint8_t>(static_cast<float>(source[0]) * scale_factor);
    uint8_t source_g =
        static_cast<uint8_t>(static_cast<float>(source[1]) * scale_factor);
    uint8_t source_b =
        static_cast<uint8_t>(static_cast<float>(source[2]) * scale_factor);
    destination[0] = source_r;
    destination[1] = source_g;
    destination[2] = source_b;
    destination[3] = source[3];
    source += 4;
    destination += 4;
  }
}
