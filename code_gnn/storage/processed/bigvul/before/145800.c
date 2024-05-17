static BrotliResult ReadContextModes(BrotliState* s) {
  BrotliBitReader* br = &s->br;
  int i = s->loop_counter;

  while (i < (int)s->num_block_types[0]) {
    uint32_t bits;
    if (!BrotliSafeReadBits(br, 2, &bits)) {
      s->loop_counter = i;
      return BROTLI_RESULT_NEEDS_MORE_INPUT;
    }
    s->context_modes[i] = (uint8_t)(bits << 1);
    BROTLI_LOG_ARRAY_INDEX(s->context_modes, i);
    i++;
  }
  return BROTLI_RESULT_SUCCESS;
}
