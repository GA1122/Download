static BROTLI_INLINE void ReadCommand(BrotliState* s, BrotliBitReader* br,
    int* insert_length) {
  ReadCommandInternal(0, s, br, insert_length);
}
