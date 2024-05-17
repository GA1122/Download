static BrotliResult ReadCodeLengthCodeLengths(BrotliState* s) {
  BrotliBitReader* br = &s->br;
  uint32_t num_codes = s->repeat;
  unsigned space = s->space;
  uint32_t i = s->sub_loop_counter;
  for (; i < CODE_LENGTH_CODES; ++i) {
    const uint8_t code_len_idx = kCodeLengthCodeOrder[i];
    uint32_t ix;
    uint32_t v;
    if (PREDICT_FALSE(!BrotliSafeGetBits(br, 4, &ix))) {
      uint32_t available_bits = BrotliGetAvailableBits(br);
      if (available_bits != 0) {
        ix = BrotliGetBitsUnmasked(br) & 0xF;
      } else {
        ix = 0;
      }
      if (kCodeLengthPrefixLength[ix] > available_bits) {
        s->sub_loop_counter = i;
        s->repeat = num_codes;
        s->space = space;
        s->substate_huffman = BROTLI_STATE_HUFFMAN_COMPLEX;
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
    }
    v = kCodeLengthPrefixValue[ix];
    BrotliDropBits(br, kCodeLengthPrefixLength[ix]);
    s->code_length_code_lengths[code_len_idx] = (uint8_t)v;
    BROTLI_LOG_ARRAY_INDEX(s->code_length_code_lengths, code_len_idx);
    if (v != 0) {
      space = space - (32U >> v);
      ++num_codes;
      ++s->code_length_histo[v];
      if (space - 1U >= 32U) {
         
        break;
      }
    }
  }
  if (!(num_codes == 1 || space == 0)) {
    return BROTLI_FAILURE();
  }
  return BROTLI_RESULT_SUCCESS;
}
