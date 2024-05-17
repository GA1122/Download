static BROTLI_NOINLINE void InverseMoveToFrontTransform(uint8_t* v,
    uint32_t v_len, BrotliState* state) {
   
  uint32_t i = 4;
  uint32_t upper_bound = state->mtf_upper_bound;
  uint8_t* mtf = state->mtf;
   
  const uint8_t b0123[4] = {0, 1, 2, 3};
  uint32_t pattern;
  memcpy(&pattern, &b0123, 4);

   
  *(uint32_t*)mtf = pattern;
  do {
    pattern += 0x04040404;  
    *(uint32_t*)(mtf + i) = pattern;
    i += 4;
  } while (i <= upper_bound);

   
  upper_bound = 0;
  for (i = 0; i < v_len; ++i) {
    int index = v[i];
    uint8_t value = mtf[index];
    upper_bound |= v[i];
    v[i] = value;
    do {
      index--;
      mtf[index + 1] = mtf[index];
    } while (index > 0);
    mtf[0] = value;
  }
   
  state->mtf_upper_bound = upper_bound;
}