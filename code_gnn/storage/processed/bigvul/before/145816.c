static BROTLI_INLINE int SafeReadSymbol(const HuffmanCode* table,
                                        BrotliBitReader* br,
                                        uint32_t* result) {
  uint32_t val;
  if (PREDICT_TRUE(BrotliSafeGetBits(br, 15, &val))) {
    *result = DecodeSymbol(val, table, br);
    return 1;
  }
  return SafeDecodeSymbol(table, br, result);
}
