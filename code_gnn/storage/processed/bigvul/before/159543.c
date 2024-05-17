void Unpack<WebGLImageConversion::kDataFormatRGBA5551, uint16_t, uint8_t>(
    const uint16_t* source,
    uint8_t* destination,
    unsigned pixels_per_row) {
#if defined(ARCH_CPU_X86_FAMILY)
  SIMD::UnpackOneRowOfRGBA5551LittleToRGBA8(source, destination,
                                            pixels_per_row);
#endif
#if WTF_CPU_ARM_NEON
  SIMD::UnpackOneRowOfRGBA5551ToRGBA8(source, destination, pixels_per_row);
#endif
#if HAVE_MIPS_MSA_INTRINSICS
  SIMD::unpackOneRowOfRGBA5551ToRGBA8MSA(source, destination, pixels_per_row);
#endif

  for (unsigned i = 0; i < pixels_per_row; ++i) {
    uint16_t packed_value = source[0];
    uint8_t r = packed_value >> 11;
    uint8_t g = (packed_value >> 6) & 0x1F;
    uint8_t b = (packed_value >> 1) & 0x1F;
    destination[0] = (r << 3) | (r & 0x7);
    destination[1] = (g << 3) | (g & 0x7);
    destination[2] = (b << 3) | (b & 0x7);
    destination[3] = (packed_value & 0x1) ? 0xFF : 0x0;
    source += 1;
    destination += 4;
  }
}
