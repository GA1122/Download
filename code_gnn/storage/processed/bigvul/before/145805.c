static BROTLI_INLINE uint32_t ReadSymbol(const HuffmanCode* table,
                                         BrotliBitReader* br) {
  return DecodeSymbol(BrotliGet16BitsUnmasked(br), table, br);
}
