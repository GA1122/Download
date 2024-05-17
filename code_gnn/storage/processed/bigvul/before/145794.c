static BROTLI_INLINE void ProcessRepeatedCodeLength(uint32_t code_len,
    uint32_t repeat_delta, uint32_t alphabet_size, uint32_t* symbol,
    uint32_t* repeat, uint32_t* space, uint32_t* prev_code_len,
    uint32_t* repeat_code_len, uint16_t* symbol_lists,
    uint16_t* code_length_histo, int* next_symbol) {
  uint32_t old_repeat;
  uint32_t new_len = 0;
  if (code_len == kCodeLengthRepeatCode) {
    new_len = *prev_code_len;
  }
  if (*repeat_code_len != new_len) {
    *repeat = 0;
    *repeat_code_len = new_len;
  }
  old_repeat = *repeat;
  if (*repeat > 0) {
    *repeat -= 2;
    *repeat <<= code_len - 14U;
  }
  *repeat += repeat_delta + 3U;
  repeat_delta = *repeat - old_repeat;
  if (*symbol + repeat_delta > alphabet_size) {
    (void)BROTLI_FAILURE();
    *symbol = alphabet_size;
    *space = 0xFFFFF;
    return;
  }
  if (*repeat_code_len != 0) {
    unsigned last = *symbol + repeat_delta;
    int next = next_symbol[*repeat_code_len];
    do {
      symbol_lists[next] = (uint16_t)*symbol;
      next = (int)*symbol;
    } while (++(*symbol) != last);
    next_symbol[*repeat_code_len] = next;
    *space -= repeat_delta << (15 - *repeat_code_len);
    code_length_histo[*repeat_code_len] = (uint16_t)
        (code_length_histo[*repeat_code_len] + repeat_delta);
  } else {
    *symbol += repeat_delta;
  }
}
