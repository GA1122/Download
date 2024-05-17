void Unpack<WebGLImageConversion::kDataFormatRGBA4444, uint16_t, uint8_t>(
    const uint16_t* source,
    uint8_t* destination,
    unsigned pixels_per_row) {
#if defined(ARCH_CPU_X86_FAMILY)
  SIMD::UnpackOneRowOfRGBA4444LittleToRGBA8(source, destination,
                                            pixels_per_row);
#endif
#if WTF_CPU_ARM_NEON
  SIMD::UnpackOneRowOfRGBA4444ToRGBA8(source, destination, pixels_per_row);
#endif
#if HAVE_MIPS_MSA_INTRINSICS
  SIMD::unpackOneRowOfRGBA4444ToRGBA8MSA(source, destination, pixels_per_row);
#endif
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    uint16_t packed_value = source[0];
    uint8_t r = packed_value >> 12;
    uint8_t g = (packed_value >> 8) & 0x0F;
    uint8_t b = (packed_value >> 4) & 0x0F;
    uint8_t a = packed_value & 0x0F;
    destination[0] = r << 4 | r;
    destination[1] = g << 4 | g;
    destination[2] = b << 4 | b;
    destination[3] = a << 4 | a;
    source += 1;
    destination += 4;
  }
}
