static BrotliResult WriteRingBuffer(size_t* available_out, uint8_t** next_out,
    size_t* total_out, BrotliState* s) {
  size_t pos = (s->pos > s->ringbuffer_size) ?
      (size_t)s->ringbuffer_size : (size_t)(s->pos);
  uint8_t* start = s->ringbuffer
      + (s->partial_pos_out & (size_t)s->ringbuffer_mask);
  size_t partial_pos_rb =
      (s->rb_roundtrips * (size_t)s->ringbuffer_size) + pos;
  size_t to_write = (partial_pos_rb - s->partial_pos_out);
  size_t num_written = *available_out;
  if (num_written > to_write) {
    num_written = to_write;
  }
  if (s->meta_block_remaining_len < 0) {
    return BROTLI_FAILURE();
  }
  memcpy(*next_out, start, num_written);
  *next_out += num_written;
  *available_out -= num_written;
  BROTLI_LOG_UINT(to_write);
  BROTLI_LOG_UINT(num_written);
  s->partial_pos_out += (size_t)num_written;
  *total_out = s->partial_pos_out;
  if (num_written < to_write) {
    return BROTLI_RESULT_NEEDS_MORE_OUTPUT;
  }
  return BROTLI_RESULT_SUCCESS;
}
