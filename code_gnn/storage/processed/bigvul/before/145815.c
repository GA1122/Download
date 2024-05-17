static BROTLI_INLINE int SafeReadDistance(BrotliState* s, BrotliBitReader* br) {
  return ReadDistanceInternal(1, s, br);
}
