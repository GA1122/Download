static BROTLI_INLINE void PreloadSymbol(int safe,
                                        const HuffmanCode* table,
                                        BrotliBitReader* br,
                                        uint32_t* bits,
                                        uint32_t* value) {
  if (safe) {
    return;
  }
  table += BrotliGetBits(br, HUFFMAN_TABLE_BITS);
  *bits = table->bits;
  *value = table->value;
}
