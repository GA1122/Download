static BROTLI_INLINE int DecodeDistanceBlockSwitchInternal(int safe,
    BrotliState* s) {
  if (!DecodeBlockTypeAndLength(safe, s, 2)) {
    return 0;
  }
  s->dist_context_map_slice =
      s->dist_context_map + (s->block_type_rb[5] << kDistanceContextBits);
  s->dist_htree_index = s->dist_context_map_slice[s->distance_context];
  return 1;
}
