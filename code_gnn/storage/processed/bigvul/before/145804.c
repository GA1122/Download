static BrotliResult ReadSimpleHuffmanSymbols(uint32_t alphabet_size,
                                             BrotliState* s) {
   
  BrotliBitReader* br = &s->br;
  uint32_t max_bits = Log2Floor(alphabet_size - 1);
  uint32_t i = s->sub_loop_counter;
  uint32_t num_symbols = s->symbol;
  while (i <= num_symbols) {
    uint32_t v;
    if (PREDICT_FALSE(!BrotliSafeReadBits(br, max_bits, &v))) {
      s->sub_loop_counter = i;
      s->substate_huffman = BROTLI_STATE_HUFFMAN_SIMPLE_READ;
      return BROTLI_RESULT_NEEDS_MORE_INPUT;
    }
    if (v >= alphabet_size) {
      return BROTLI_FAILURE();
    }
    s->symbols_lists_array[i] = (uint16_t)v;
    BROTLI_LOG_UINT(s->symbols_lists_array[i]);
    ++i;
  }

  for (i = 0; i < num_symbols; ++i) {
    uint32_t k = i + 1;
    for (; k <= num_symbols; ++k) {
      if (s->symbols_lists_array[i] == s->symbols_lists_array[k]) {
        return BROTLI_FAILURE();
      }
    }
  }

  return BROTLI_RESULT_SUCCESS;
}
