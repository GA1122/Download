static BROTLI_INLINE uint32_t ReadPreloadedSymbol(const HuffmanCode* table,
                                                  BrotliBitReader* br,
                                                  uint32_t* bits,
                                                  uint32_t* value) {
  uint32_t result = *value;
  if (PREDICT_FALSE(*bits > HUFFMAN_TABLE_BITS)) {
    uint32_t val = BrotliGet16BitsUnmasked(br);
    const HuffmanCode* ext = table + (val & HUFFMAN_TABLE_MASK) + *value;
    uint32_t mask = BitMask((*bits - HUFFMAN_TABLE_BITS));
    BrotliDropBits(br, HUFFMAN_TABLE_BITS);
    ext += (val >> HUFFMAN_TABLE_BITS) & mask;
    BrotliDropBits(br, ext->bits);
    result = ext->value;
  } else {
    BrotliDropBits(br, *bits);
  }
  PreloadSymbol(0, table, br, bits, value);
  return result;
}
