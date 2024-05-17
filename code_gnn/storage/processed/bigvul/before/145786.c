static BROTLI_INLINE uint32_t DecodeSymbol(uint32_t bits,
                                           const HuffmanCode* table,
                                           BrotliBitReader* br) {
  table += bits & HUFFMAN_TABLE_MASK;
  if (table->bits > HUFFMAN_TABLE_BITS) {
    uint32_t nbits = table->bits - HUFFMAN_TABLE_BITS;
    BrotliDropBits(br, HUFFMAN_TABLE_BITS);
    table += table->value;
    table += (bits >> HUFFMAN_TABLE_BITS) & BitMask(nbits);
  }
  BrotliDropBits(br, table->bits);
  return table->value;
}
