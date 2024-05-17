static BROTLI_INLINE BROTLI_NO_ASAN void memmove16(
    uint8_t* dst, uint8_t* src) {
#if BROTLI_SAFE_MEMMOVE
   
  uint32_t buffer[4];
  memcpy(buffer, src, 16);
  memcpy(dst, buffer, 16);
#elif defined(__ARM_NEON__)
  vst1q_u8(dst, vld1q_u8(src));
#else
   
  memcpy(dst, src, 16);
#endif
}
