static BrotliResult ReadHuffmanCode(uint32_t alphabet_size,
                                    HuffmanCode* table,
                                    uint32_t* opt_table_size,
                                    BrotliState* s) {
  BrotliBitReader* br = &s->br;
   
  alphabet_size &= 0x3ff;
   
  switch (s->substate_huffman) {
    case BROTLI_STATE_HUFFMAN_NONE:
      if (!BrotliSafeReadBits(br, 2, &s->sub_loop_counter)) {
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      BROTLI_LOG_UINT(s->sub_loop_counter);
       
      if (s->sub_loop_counter != 1) {
        s->space = 32;
        s->repeat = 0;  
        memset(&s->code_length_histo[0], 0, sizeof(s->code_length_histo[0]) *
            (BROTLI_HUFFMAN_MAX_CODE_LENGTH_CODE_LENGTH + 1));
        memset(&s->code_length_code_lengths[0], 0,
            sizeof(s->code_length_code_lengths));
        s->substate_huffman = BROTLI_STATE_HUFFMAN_COMPLEX;
        goto Complex;
      }
       

    case BROTLI_STATE_HUFFMAN_SIMPLE_SIZE:
       
      if (!BrotliSafeReadBits(br, 2, &s->symbol)) {  
        s->substate_huffman = BROTLI_STATE_HUFFMAN_SIMPLE_SIZE;
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      s->sub_loop_counter = 0;
       
    case BROTLI_STATE_HUFFMAN_SIMPLE_READ: {
      BrotliResult result = ReadSimpleHuffmanSymbols(alphabet_size, s);
      if (result != BROTLI_RESULT_SUCCESS) {
        return result;
      }
       
    }
    case BROTLI_STATE_HUFFMAN_SIMPLE_BUILD: {
      uint32_t table_size;
      if (s->symbol == 3) {
        uint32_t bits;
        if (!BrotliSafeReadBits(br, 1, &bits)) {
          s->substate_huffman = BROTLI_STATE_HUFFMAN_SIMPLE_BUILD;
          return BROTLI_RESULT_NEEDS_MORE_INPUT;
        }
        s->symbol += bits;
      }
      BROTLI_LOG_UINT(s->symbol);
      table_size = BrotliBuildSimpleHuffmanTable(
          table, HUFFMAN_TABLE_BITS, s->symbols_lists_array, s->symbol);
      if (opt_table_size) {
        *opt_table_size = table_size;
      }
      s->substate_huffman = BROTLI_STATE_HUFFMAN_NONE;
      return BROTLI_RESULT_SUCCESS;
    }

Complex:  
    case BROTLI_STATE_HUFFMAN_COMPLEX: {
      uint32_t i;
      BrotliResult result = ReadCodeLengthCodeLengths(s);
      if (result != BROTLI_RESULT_SUCCESS) {
        return result;
      }
      BrotliBuildCodeLengthsHuffmanTable(s->table,
                                         s->code_length_code_lengths,
                                         s->code_length_histo);
      memset(&s->code_length_histo[0], 0, sizeof(s->code_length_histo));
      for (i = 0; i <= BROTLI_HUFFMAN_MAX_CODE_LENGTH; ++i) {
        s->next_symbol[i] = (int)i - (BROTLI_HUFFMAN_MAX_CODE_LENGTH + 1);
        s->symbol_lists[(int)i - (BROTLI_HUFFMAN_MAX_CODE_LENGTH + 1)] = 0xFFFF;
      }

      s->symbol = 0;
      s->prev_code_len = kDefaultCodeLength;
      s->repeat = 0;
      s->repeat_code_len = 0;
      s->space = 32768;
      s->substate_huffman = BROTLI_STATE_HUFFMAN_LENGTH_SYMBOLS;
       
    }
    case BROTLI_STATE_HUFFMAN_LENGTH_SYMBOLS: {
      uint32_t table_size;
      BrotliResult result = ReadSymbolCodeLengths(alphabet_size, s);
      if (result == BROTLI_RESULT_NEEDS_MORE_INPUT) {
        result = SafeReadSymbolCodeLengths(alphabet_size, s);
      }
      if (result != BROTLI_RESULT_SUCCESS) {
        return result;
      }

      if (s->space != 0) {
        BROTLI_LOG(("[ReadHuffmanCode] space = %d\n", s->space));
        return BROTLI_FAILURE();
      }
      table_size = BrotliBuildHuffmanTable(table, HUFFMAN_TABLE_BITS,
          s->symbol_lists, s->code_length_histo);
      if (opt_table_size) {
        *opt_table_size = table_size;
      }
      s->substate_huffman = BROTLI_STATE_HUFFMAN_NONE;
      return BROTLI_RESULT_SUCCESS;
    }

    default:
      return BROTLI_FAILURE();
  }
}
