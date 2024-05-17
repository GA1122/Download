static BrotliResult SafeReadSymbolCodeLengths(
    uint32_t alphabet_size, BrotliState* s) {
  BrotliBitReader* br = &s->br;
  while (s->symbol < alphabet_size && s->space > 0) {
    const HuffmanCode* p = s->table;
    uint32_t code_len;
    uint32_t bits = 0;
    uint32_t available_bits = BrotliGetAvailableBits(br);
    if (available_bits != 0) {
      bits = (uint32_t)BrotliGetBitsUnmasked(br);
    }
    p += bits & BitMask(BROTLI_HUFFMAN_MAX_CODE_LENGTH_CODE_LENGTH);
    if (p->bits > available_bits) goto pullMoreInput;
    code_len = p->value;  
    if (code_len < kCodeLengthRepeatCode) {
      BrotliDropBits(br, p->bits);
      ProcessSingleCodeLength(code_len, &s->symbol, &s->repeat, &s->space,
          &s->prev_code_len, s->symbol_lists, s->code_length_histo,
          s->next_symbol);
    } else {  
      uint32_t extra_bits = code_len - 14U;
      uint32_t repeat_delta = (bits >> p->bits) & BitMask(extra_bits);
      if (available_bits < p->bits + extra_bits) goto pullMoreInput;
      BrotliDropBits(br, p->bits + extra_bits);
      ProcessRepeatedCodeLength(code_len, repeat_delta, alphabet_size,
          &s->symbol, &s->repeat, &s->space, &s->prev_code_len,
          &s->repeat_code_len, s->symbol_lists, s->code_length_histo,
          s->next_symbol);
    }
    continue;

pullMoreInput:
    if (!BrotliPullByte(br)) {
      return BROTLI_RESULT_NEEDS_MORE_INPUT;
    }
  }
  return BROTLI_RESULT_SUCCESS;
}
