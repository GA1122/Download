size_t QuicStreamSequencerBuffer::ReadableBytes() const {
  return gaps_.front().begin_offset - total_bytes_read_;
}
