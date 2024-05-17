static BROTLI_INLINE int CheckInputAmount(int safe,
    BrotliBitReader* const br, size_t num) {
  if (safe) {
    return 1;
  }
  return BrotliCheckInputAmount(br, num);
}
