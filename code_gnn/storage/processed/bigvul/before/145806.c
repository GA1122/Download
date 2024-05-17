static BrotliResult ReadSymbolCodeLengths(
    uint32_t alphabet_size, BrotliState* s) {
  BrotliBitReader* br = &s->br;
  uint32_t symbol = s->symbol;
  uint32_t repeat = s->repeat;
  uint32_t space = s->space;
  uint32_t prev_code_len = s->prev_code_len;
  uint32_t repeat_code_len = s->repeat_code_len;
  uint16_t* symbol_lists = s->symbol_lists;
  uint16_t* code_length_histo = s->code_length_histo;
  int* next_symbol = s->next_symbol;
  if (!BrotliWarmupBitReader(br)) {
    return BROTLI_RESULT_NEEDS_MORE_INPUT;
  }
  while (symbol < alphabet_size && space > 0) {
    const HuffmanCode* p = s->table;
    uint32_t code_len;
    if (!BrotliCheckInputAmount(br, BROTLI_SHORT_FILL_BIT_WINDOW_READ)) {
      s->symbol = symbol;
      s->repeat = repeat;
      s->prev_code_len = prev_code_len;
      s->repeat_code_len = repeat_code_len;
      s->space = space;
      return BROTLI_RESULT_NEEDS_MORE_INPUT;
    }
    BrotliFillBitWindow16(br);
    p += BrotliGetBitsUnmasked(br) &
        BitMask(BROTLI_HUFFMAN_MAX_CODE_LENGTH_CODE_LENGTH);
    BrotliDropBits(br, p->bits);  
    code_len = p->value;  
    if (code_len < kCodeLengthRepeatCode) {
      ProcessSingleCodeLength(code_len, &symbol, &repeat, &space,
          &prev_code_len, symbol_lists, code_length_histo, next_symbol);
    } else {  
      uint32_t repeat_delta =
          (uint32_t)BrotliGetBitsUnmasked(br) & BitMask(code_len - 14U);
      BrotliDropBits(br, code_len - 14U);
      ProcessRepeatedCodeLength(code_len, repeat_delta, alphabet_size,
          &symbol, &repeat, &space, &prev_code_len, &repeat_code_len,
          symbol_lists, code_length_histo, next_symbol);
    }
  }
  s->space = space;
  return BROTLI_RESULT_SUCCESS;
}
