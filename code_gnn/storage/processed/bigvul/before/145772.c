int BrotliDecompressedSize(size_t encoded_size,
                           const uint8_t* encoded_buffer,
                           size_t* decoded_size) {
  BrotliState s;
  int next_block_header;
  BrotliStateInit(&s);
  s.br.next_in = encoded_buffer;
  s.br.avail_in = encoded_size;
  if (!BrotliWarmupBitReader(&s.br)) {
    return 0;
  }
  DecodeWindowBits(&s.br);
  if (DecodeMetaBlockLength(&s, &s.br) != BROTLI_RESULT_SUCCESS) {
    return 0;
  }
  *decoded_size = (size_t)s.meta_block_remaining_len;
  if (s.is_last_metablock) {
    return 1;
  }
  if (!s.is_uncompressed || !BrotliJumpToByteBoundary(&s.br)) {
    return 0;
  }
  next_block_header = BrotliPeekByte(&s.br, (size_t)s.meta_block_remaining_len);
  return (next_block_header != -1) && ((next_block_header & 3) == 3);
}
