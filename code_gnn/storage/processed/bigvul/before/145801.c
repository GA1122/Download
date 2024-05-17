static BROTLI_INLINE int ReadDistanceInternal(int safe,
    BrotliState* s, BrotliBitReader* br) {
  int distval;
  BrotliBitReaderState memento;
  HuffmanCode* distance_tree = s->distance_hgroup.htrees[s->dist_htree_index];
  if (!safe) {
    s->distance_code = (int)ReadSymbol(distance_tree, br);
  } else {
    uint32_t code;
    BrotliBitReaderSaveState(br, &memento);
    if (!SafeReadSymbol(distance_tree, br, &code)) {
      return 0;
    }
    s->distance_code = (int)code;
  }
   
   
  if ((s->distance_code & ~0xf) == 0) {
    TakeDistanceFromRingBuffer(s);
    --s->block_length[2];
    return 1;
  }
  distval = s->distance_code - (int)s->num_direct_distance_codes;
  if (distval >= 0) {
    uint32_t nbits;
    int postfix;
    int offset;
    if (!safe && (s->distance_postfix_bits == 0)) {
      nbits = ((uint32_t)distval >> 1) + 1;
      offset = ((2 + (distval & 1)) << nbits) - 4;
      s->distance_code = (int)s->num_direct_distance_codes +
          offset + (int)BrotliReadBits(br, nbits);
    } else {
       
      uint32_t bits;
      postfix = distval & s->distance_postfix_mask;
      distval >>= s->distance_postfix_bits;
      nbits = ((uint32_t)distval >> 1) + 1;
      if (safe) {
        if (!SafeReadBits(br, nbits, &bits)) {
          s->distance_code = -1;  
          BrotliBitReaderRestoreState(br, &memento);
          return 0;
        }
      } else {
        bits = BrotliReadBits(br, nbits);
      }
      offset = ((2 + (distval & 1)) << nbits) - 4;
      s->distance_code = (int)s->num_direct_distance_codes +
          ((offset + (int)bits) << s->distance_postfix_bits) + postfix;
    }
  }
  s->distance_code = s->distance_code - NUM_DISTANCE_SHORT_CODES + 1;
  --s->block_length[2];
  return 1;
}
