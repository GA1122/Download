size_t QuicStreamSequencerBuffer::FlushBufferedFrames() {
  size_t prev_total_bytes_read = total_bytes_read_;
  total_bytes_read_ = gaps_.back().begin_offset;
  Clear();
  return total_bytes_read_ - prev_total_bytes_read;
}
