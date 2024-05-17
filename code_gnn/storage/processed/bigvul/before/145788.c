static uint32_t DecodeWindowBits(BrotliBitReader* br) {
  uint32_t n;
  BrotliTakeBits(br, 1, &n);
  if (n == 0) {
    return 16;
  }
  BrotliTakeBits(br, 3, &n);
  if (n != 0) {
    return 17 + n;
  }
  BrotliTakeBits(br, 3, &n);
  if (n != 0) {
    return 8 + n;
  }
  return 17;
}
