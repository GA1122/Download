static BrotliResult DecodeContextMap(uint32_t context_map_size,
                                     uint32_t* num_htrees,
                                     uint8_t** context_map_arg,
                                     BrotliState* s) {
  BrotliBitReader* br = &s->br;
  BrotliResult result = BROTLI_RESULT_SUCCESS;

  switch((int)s->substate_context_map) {
    case BROTLI_STATE_CONTEXT_MAP_NONE:
      result = DecodeVarLenUint8(s, br, num_htrees);
      if (result != BROTLI_RESULT_SUCCESS) {
        return result;
      }
      (*num_htrees)++;
      s->context_index = 0;
      BROTLI_LOG_UINT(context_map_size);
      BROTLI_LOG_UINT(*num_htrees);
      *context_map_arg = (uint8_t*)BROTLI_ALLOC(s, (size_t)context_map_size);
      if (*context_map_arg == 0) {
        return BROTLI_FAILURE();
      }
      if (*num_htrees <= 1) {
        memset(*context_map_arg, 0, (size_t)context_map_size);
        return BROTLI_RESULT_SUCCESS;
      }
      s->substate_context_map = BROTLI_STATE_CONTEXT_MAP_READ_PREFIX;
       
    case BROTLI_STATE_CONTEXT_MAP_READ_PREFIX: {
      uint32_t bits;
       
      if (!BrotliSafeGetBits(br, 5, &bits)) {
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      if ((bits & 1) != 0) {  
        s->max_run_length_prefix = (bits >> 1) + 1;
        BrotliDropBits(br, 5);
      } else {
        s->max_run_length_prefix = 0;
        BrotliDropBits(br, 1);
      }
      BROTLI_LOG_UINT(s->max_run_length_prefix);
      s->substate_context_map = BROTLI_STATE_CONTEXT_MAP_HUFFMAN;
       
    }
    case BROTLI_STATE_CONTEXT_MAP_HUFFMAN:
      result = ReadHuffmanCode(*num_htrees + s->max_run_length_prefix,
                               s->context_map_table, NULL, s);
      if (result != BROTLI_RESULT_SUCCESS) return result;
      s->code = 0xFFFF;
      s->substate_context_map = BROTLI_STATE_CONTEXT_MAP_DECODE;
       
    case BROTLI_STATE_CONTEXT_MAP_DECODE: {
      uint32_t context_index = s->context_index;
      uint32_t max_run_length_prefix = s->max_run_length_prefix;
      uint8_t* context_map = *context_map_arg;
      uint32_t code = s->code;
      if (code != 0xFFFF) {
        goto rleCode;
      }
      while (context_index < context_map_size) {
        if (!SafeReadSymbol(s->context_map_table, br, &code)) {
          s->code = 0xFFFF;
          s->context_index = context_index;
          return BROTLI_RESULT_NEEDS_MORE_INPUT;
        }
        BROTLI_LOG_UINT(code);

        if (code == 0) {
          context_map[context_index++] = 0;
          continue;
        }
        if (code > max_run_length_prefix) {
          context_map[context_index++] =
              (uint8_t)(code - max_run_length_prefix);
          continue;
        }
rleCode:
        {
          uint32_t reps;
          if (!BrotliSafeReadBits(br, code, &reps)) {
            s->code = code;
            s->context_index = context_index;
            return BROTLI_RESULT_NEEDS_MORE_INPUT;
          }
          reps += 1U << code;
          BROTLI_LOG_UINT(reps);
          if (context_index + reps > context_map_size) {
            return BROTLI_FAILURE();
          }
          do {
            context_map[context_index++] = 0;
          } while (--reps);
        }
      }
       
    }
    case BROTLI_STATE_CONTEXT_MAP_TRANSFORM: {
      uint32_t bits;
      if (!BrotliSafeReadBits(br, 1, &bits)) {
        s->substate_context_map = BROTLI_STATE_CONTEXT_MAP_TRANSFORM;
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      if (bits != 0) {
        InverseMoveToFrontTransform(*context_map_arg, context_map_size, s);
      }
      s->substate_context_map = BROTLI_STATE_CONTEXT_MAP_NONE;
      return BROTLI_RESULT_SUCCESS;
    }
  }

  return BROTLI_FAILURE();
}
