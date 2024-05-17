void Unpack<WebGLImageConversion::kDataFormatBGRA8, uint8_t, uint8_t>(
    const uint8_t* source,
    uint8_t* destination,
    unsigned pixels_per_row) {
  const uint32_t* source32 = reinterpret_cast_ptr<const uint32_t*>(source);
  uint32_t* destination32 = reinterpret_cast_ptr<uint32_t*>(destination);

#if defined(ARCH_CPU_X86_FAMILY)
  SIMD::UnpackOneRowOfBGRA8LittleToRGBA8(source32, destination32,
                                         pixels_per_row);
#endif
#if HAVE_MIPS_MSA_INTRINSICS
  SIMD::unpackOneRowOfBGRA8LittleToRGBA8MSA(source32, destination32,
                                            pixels_per_row);
#endif
  for (unsigned i = 0; i < pixels_per_row; ++i) {
    uint32_t bgra = source32[i];
#if defined(ARCH_CPU_BIG_ENDIAN)
    uint32_t brMask = 0xff00ff00;
    uint32_t gaMask = 0x00ff00ff;
#else
    uint32_t br_mask = 0x00ff00ff;
    uint32_t ga_mask = 0xff00ff00;
#endif
    uint32_t rgba =
        (((bgra >> 16) | (bgra << 16)) & br_mask) | (bgra & ga_mask);
    destination32[i] = rgba;
  }
}
