static BROTLI_INLINE int SafeReadCommand(BrotliState* s, BrotliBitReader* br,
    int* insert_length) {
  return ReadCommandInternal(1, s, br, insert_length);
}
