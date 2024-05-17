static BROTLI_NOINLINE BrotliResult DecodeVarLenUint8(BrotliState* s,
    BrotliBitReader* br, uint32_t* value) {
  uint32_t bits;
  switch (s->substate_decode_uint8) {
    case BROTLI_STATE_DECODE_UINT8_NONE:
      if (PREDICT_FALSE(!BrotliSafeReadBits(br, 1, &bits))) {
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      if (bits == 0) {
        *value = 0;
        return BROTLI_RESULT_SUCCESS;
      }
       

    case BROTLI_STATE_DECODE_UINT8_SHORT:
      if (PREDICT_FALSE(!BrotliSafeReadBits(br, 3, &bits))) {
        s->substate_decode_uint8 = BROTLI_STATE_DECODE_UINT8_SHORT;
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      if (bits == 0) {
        *value = 1;
        s->substate_decode_uint8 = BROTLI_STATE_DECODE_UINT8_NONE;
        return BROTLI_RESULT_SUCCESS;
      }
       
      *value = bits;
       

    case BROTLI_STATE_DECODE_UINT8_LONG:
      if (PREDICT_FALSE(!BrotliSafeReadBits(br, *value, &bits))) {
        s->substate_decode_uint8 = BROTLI_STATE_DECODE_UINT8_LONG;
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      *value = (1U << *value) + bits;
      s->substate_decode_uint8 = BROTLI_STATE_DECODE_UINT8_NONE;
      return BROTLI_RESULT_SUCCESS;

    default:
      return BROTLI_FAILURE();
  }
}
