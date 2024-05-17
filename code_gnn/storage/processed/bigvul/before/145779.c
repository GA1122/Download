static BROTLI_INLINE int DecodeCommandBlockSwitchInternal(int safe,
    BrotliState* s) {
  if (!DecodeBlockTypeAndLength(safe, s, 1)) {
    return 0;
  }
  s->htree_command = s->insert_copy_hgroup.htrees[s->block_type_rb[3]];
  return 1;
}
