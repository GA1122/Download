static BROTLI_INLINE int WarmupBitReader(int safe, BrotliBitReader* const br) {
  if (safe) {
    return 1;
  }
  return BrotliWarmupBitReader(br);
}
