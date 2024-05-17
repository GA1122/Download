BrotliResult BrotliDecompressStream(size_t* available_in,
    const uint8_t** next_in, size_t* available_out, uint8_t** next_out,
    size_t* total_out, BrotliState* s) {
  BrotliResult result = BROTLI_RESULT_SUCCESS;
  BrotliBitReader* br = &s->br;
  if (s->buffer_length == 0) {  
    br->avail_in = *available_in;
    br->next_in = *next_in;
  } else {
     
    result = BROTLI_RESULT_NEEDS_MORE_INPUT;
    br->next_in = &s->buffer.u8[0];
  }
   
  for (;;) {
    if (result != BROTLI_RESULT_SUCCESS) {  
      if (result == BROTLI_RESULT_NEEDS_MORE_INPUT) {
        if (s->ringbuffer != 0) {  
          WriteRingBuffer(available_out, next_out, total_out, s);
        }
        if (s->buffer_length != 0) {  
          if (br->avail_in == 0) {  
             
            s->buffer_length = 0;
             
            result = BROTLI_RESULT_SUCCESS;
            br->avail_in = *available_in;
            br->next_in = *next_in;
            continue;
          } else if (*available_in != 0) {
             
            result = BROTLI_RESULT_SUCCESS;
            s->buffer.u8[s->buffer_length] = **next_in;
            s->buffer_length++;
            br->avail_in = s->buffer_length;
            (*next_in)++;
            (*available_in)--;
             
            continue;
          }
           
          break;
        } else {  
           
          *next_in = br->next_in;
          *available_in = br->avail_in;
          while (*available_in) {
            s->buffer.u8[s->buffer_length] = **next_in;
            s->buffer_length++;
            (*next_in)++;
            (*available_in)--;
          }
          break;
        }
         
      }

       

      if (s->buffer_length != 0) {
         
        s->buffer_length = 0;
      } else {
         
        BrotliBitReaderUnload(br);
        *available_in = br->avail_in;
        *next_in = br->next_in;
      }
      break;
    }
    switch (s->state) {
      case BROTLI_STATE_UNINITED:
         
        if (!BrotliWarmupBitReader(br)) {
          result = BROTLI_RESULT_NEEDS_MORE_INPUT;
          break;
        }
         
        s->window_bits = DecodeWindowBits(br);  
        BROTLI_LOG_UINT(s->window_bits);
        if (s->window_bits == 9) {
           
          result = BROTLI_FAILURE();
          break;
        }
        s->max_backward_distance = (1 << s->window_bits) - 16;
        s->max_backward_distance_minus_custom_dict_size =
            s->max_backward_distance - s->custom_dict_size;

         
        s->block_type_trees = (HuffmanCode*)BROTLI_ALLOC(s,
            6 * BROTLI_HUFFMAN_MAX_TABLE_SIZE * sizeof(HuffmanCode));
        if (s->block_type_trees == 0) {
          result = BROTLI_FAILURE();
          break;
        }
        s->block_len_trees = s->block_type_trees +
            3 * BROTLI_HUFFMAN_MAX_TABLE_SIZE;

        s->state = BROTLI_STATE_METABLOCK_BEGIN;
         
      case BROTLI_STATE_METABLOCK_BEGIN:
        BrotliStateMetablockBegin(s);
        BROTLI_LOG_UINT(s->pos);
        s->state = BROTLI_STATE_METABLOCK_HEADER;
         
      case BROTLI_STATE_METABLOCK_HEADER:
        result = DecodeMetaBlockLength(s, br);  
        if (result != BROTLI_RESULT_SUCCESS) {
          break;
        }
        BROTLI_LOG_UINT(s->is_last_metablock);
        BROTLI_LOG_UINT(s->meta_block_remaining_len);
        BROTLI_LOG_UINT(s->is_metadata);
        BROTLI_LOG_UINT(s->is_uncompressed);
        if (s->is_metadata || s->is_uncompressed) {
          if (!BrotliJumpToByteBoundary(br)) {
            result = BROTLI_FAILURE();
            break;
          }
        }
        if (s->is_metadata) {
          s->state = BROTLI_STATE_METADATA;
          break;
        }
        if (s->meta_block_remaining_len == 0) {
          s->state = BROTLI_STATE_METABLOCK_DONE;
          break;
        }
        if (!s->ringbuffer) {
          if (!BrotliAllocateRingBuffer(s, br)) {
            result = BROTLI_FAILURE();
            break;
          }
        }
        if (s->is_uncompressed) {
          s->state = BROTLI_STATE_UNCOMPRESSED;
          break;
        }
        s->loop_counter = 0;
        s->state = BROTLI_STATE_HUFFMAN_CODE_0;
        break;
      case BROTLI_STATE_UNCOMPRESSED: {
        int bytes_copied = s->meta_block_remaining_len;
        result = CopyUncompressedBlockToOutput(
            available_out, next_out, total_out, s);
        bytes_copied -= s->meta_block_remaining_len;
        if (result != BROTLI_RESULT_SUCCESS) {
          break;
        }
        s->state = BROTLI_STATE_METABLOCK_DONE;
        break;
      }
      case BROTLI_STATE_METADATA:
        for (; s->meta_block_remaining_len > 0; --s->meta_block_remaining_len) {
          uint32_t bits;
           
          if (!BrotliSafeReadBits(br, 8, &bits)) {
            result = BROTLI_RESULT_NEEDS_MORE_INPUT;
            break;
          }
        }
        if (result == BROTLI_RESULT_SUCCESS) {
          s->state = BROTLI_STATE_METABLOCK_DONE;
        }
        break;
      case BROTLI_STATE_HUFFMAN_CODE_0:
        if (s->loop_counter >= 3) {
          s->state = BROTLI_STATE_METABLOCK_HEADER_2;
          break;
        }
         
        result = DecodeVarLenUint8(s, br, &s->num_block_types[s->loop_counter]);
        if (result != BROTLI_RESULT_SUCCESS) {
          break;
        }
        s->num_block_types[s->loop_counter]++;
        BROTLI_LOG_UINT(s->num_block_types[s->loop_counter]);
        if (s->num_block_types[s->loop_counter] < 2) {
          s->loop_counter++;
          break;
        }
        s->state = BROTLI_STATE_HUFFMAN_CODE_1;
         
      case BROTLI_STATE_HUFFMAN_CODE_1: {
        int tree_offset = s->loop_counter * BROTLI_HUFFMAN_MAX_TABLE_SIZE;
        result = ReadHuffmanCode(s->num_block_types[s->loop_counter] + 2,
            &s->block_type_trees[tree_offset], NULL, s);
        if (result != BROTLI_RESULT_SUCCESS) break;
        s->state = BROTLI_STATE_HUFFMAN_CODE_2;
         
      }
      case BROTLI_STATE_HUFFMAN_CODE_2: {
        int tree_offset = s->loop_counter * BROTLI_HUFFMAN_MAX_TABLE_SIZE;
        result = ReadHuffmanCode(kNumBlockLengthCodes,
            &s->block_len_trees[tree_offset], NULL, s);
        if (result != BROTLI_RESULT_SUCCESS) break;
        s->state = BROTLI_STATE_HUFFMAN_CODE_3;
         
      }
      case BROTLI_STATE_HUFFMAN_CODE_3: {
        int tree_offset = s->loop_counter * BROTLI_HUFFMAN_MAX_TABLE_SIZE;
        if (!SafeReadBlockLength(s, &s->block_length[s->loop_counter],
            &s->block_len_trees[tree_offset], br)) {
          result = BROTLI_RESULT_NEEDS_MORE_INPUT;
          break;
        }
        BROTLI_LOG_UINT(s->block_length[s->loop_counter]);
        s->loop_counter++;
        s->state = BROTLI_STATE_HUFFMAN_CODE_0;
        break;
      }
      case BROTLI_STATE_METABLOCK_HEADER_2: {
        uint32_t bits;
        if (!BrotliSafeReadBits(br, 6, &bits)) {
          result = BROTLI_RESULT_NEEDS_MORE_INPUT;
          break;
        }
        s->distance_postfix_bits = bits & BitMask(2);
        bits >>= 2;
        s->num_direct_distance_codes = NUM_DISTANCE_SHORT_CODES +
            (bits << s->distance_postfix_bits);
        BROTLI_LOG_UINT(s->num_direct_distance_codes);
        BROTLI_LOG_UINT(s->distance_postfix_bits);
        s->distance_postfix_mask = (int)BitMask(s->distance_postfix_bits);
        s->context_modes =
            (uint8_t*)BROTLI_ALLOC(s, (size_t)s->num_block_types[0]);
        if (s->context_modes == 0) {
          result = BROTLI_FAILURE();
          break;
        }
        s->loop_counter = 0;
        s->state = BROTLI_STATE_CONTEXT_MODES;
         
      }
      case BROTLI_STATE_CONTEXT_MODES:
        result = ReadContextModes(s);
        if (result != BROTLI_RESULT_SUCCESS) {
          break;
        }
        s->state = BROTLI_STATE_CONTEXT_MAP_1;
         
      case BROTLI_STATE_CONTEXT_MAP_1: {
        uint32_t j;
        result = DecodeContextMap(s->num_block_types[0] << kLiteralContextBits,
                                  &s->num_literal_htrees, &s->context_map, s);
        if (result != BROTLI_RESULT_SUCCESS) {
          break;
        }
        s->trivial_literal_context = 1;
        for (j = 0; j < s->num_block_types[0] << kLiteralContextBits; j++) {
          if (s->context_map[j] != j >> kLiteralContextBits) {
            s->trivial_literal_context = 0;
            break;
          }
        }
        s->state = BROTLI_STATE_CONTEXT_MAP_2;
         
      }
      case BROTLI_STATE_CONTEXT_MAP_2:
        {
          uint32_t num_distance_codes =
              s->num_direct_distance_codes + (48U << s->distance_postfix_bits);
          result = DecodeContextMap(
              s->num_block_types[2] << kDistanceContextBits,
              &s->num_dist_htrees, &s->dist_context_map, s);
          if (result != BROTLI_RESULT_SUCCESS) {
            break;
          }
          BrotliHuffmanTreeGroupInit(s, &s->literal_hgroup, kNumLiteralCodes,
                                     s->num_literal_htrees);
          BrotliHuffmanTreeGroupInit(s, &s->insert_copy_hgroup,
                                     kNumInsertAndCopyCodes,
              s->num_block_types[1]);
          BrotliHuffmanTreeGroupInit(s, &s->distance_hgroup, num_distance_codes,
                                     s->num_dist_htrees);
          if (s->literal_hgroup.codes == 0 ||
              s->insert_copy_hgroup.codes == 0 ||
              s->distance_hgroup.codes == 0) {
            return BROTLI_FAILURE();
          }
        }
        s->loop_counter = 0;
        s->state = BROTLI_STATE_TREE_GROUP;
         
      case BROTLI_STATE_TREE_GROUP:
        {
          HuffmanTreeGroup* hgroup = NULL;
          switch (s->loop_counter) {
            case 0:
              hgroup = &s->literal_hgroup;
              break;
            case 1:
              hgroup = &s->insert_copy_hgroup;
              break;
            case 2:
              hgroup = &s->distance_hgroup;
              break;
          }
          result = HuffmanTreeGroupDecode(hgroup, s);
        }
        if (result != BROTLI_RESULT_SUCCESS) break;
        s->loop_counter++;
        if (s->loop_counter >= 3) {
          uint8_t context_mode = s->context_modes[s->block_type_rb[1]];
          s->context_map_slice = s->context_map;
          s->dist_context_map_slice = s->dist_context_map;
          s->context_lookup1 =
              &kContextLookup[kContextLookupOffsets[context_mode]];
          s->context_lookup2 =
              &kContextLookup[kContextLookupOffsets[context_mode + 1]];
          s->htree_command = s->insert_copy_hgroup.htrees[0];
          s->literal_htree = s->literal_hgroup.htrees[s->literal_htree_index];
          s->state = BROTLI_STATE_COMMAND_BEGIN;
        }
        break;
      case BROTLI_STATE_COMMAND_BEGIN:
      case BROTLI_STATE_COMMAND_INNER:
      case BROTLI_STATE_COMMAND_POST_DECODE_LITERALS:
      case BROTLI_STATE_COMMAND_POST_WRAP_COPY:
        result = ProcessCommands(s);
        if (result == BROTLI_RESULT_NEEDS_MORE_INPUT) {
          result = SafeProcessCommands(s);
        }
        break;
      case BROTLI_STATE_COMMAND_INNER_WRITE:
      case BROTLI_STATE_COMMAND_POST_WRITE_1:
      case BROTLI_STATE_COMMAND_POST_WRITE_2:
        result = WriteRingBuffer(available_out, next_out, total_out, s);
        if (result != BROTLI_RESULT_SUCCESS) {
          break;
        }
        s->pos -= s->ringbuffer_size;
        s->rb_roundtrips++;
        s->max_distance = s->max_backward_distance;
        if (s->state == BROTLI_STATE_COMMAND_POST_WRITE_1) {
          memcpy(s->ringbuffer, s->ringbuffer_end, (size_t)s->pos);
          if (s->meta_block_remaining_len <= 0) {
             
            s->state = BROTLI_STATE_METABLOCK_DONE;
          } else {
            s->state = BROTLI_STATE_COMMAND_BEGIN;
          }
          break;
        } else if (s->state == BROTLI_STATE_COMMAND_POST_WRITE_2) {
          s->state = BROTLI_STATE_COMMAND_POST_WRAP_COPY;
        } else {   
          if (s->loop_counter == 0) {
            if (s->meta_block_remaining_len <= 0) {
              s->state = BROTLI_STATE_METABLOCK_DONE;
            } else {
              s->state = BROTLI_STATE_COMMAND_POST_DECODE_LITERALS;
            }
            break;
          }
          s->state = BROTLI_STATE_COMMAND_INNER;
        }
        break;
      case BROTLI_STATE_METABLOCK_DONE:
        BrotliStateCleanupAfterMetablock(s);
        if (!s->is_last_metablock) {
          s->state = BROTLI_STATE_METABLOCK_BEGIN;
          break;
        }
        if (!BrotliJumpToByteBoundary(br)) {
          result = BROTLI_FAILURE();
        }
        if (s->buffer_length == 0) {
          BrotliBitReaderUnload(br);
          *available_in = br->avail_in;
          *next_in = br->next_in;
        }
        s->state = BROTLI_STATE_DONE;
         
      case BROTLI_STATE_DONE:
        if (s->ringbuffer != 0) {
          result = WriteRingBuffer(available_out, next_out, total_out, s);
          if (result != BROTLI_RESULT_SUCCESS) {
            break;
          }
        }
        return result;
    }
  }
  return result;
}
