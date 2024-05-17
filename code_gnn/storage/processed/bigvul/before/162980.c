bool QuicStreamSequencerBuffer::Empty() const {
  return gaps_.size() == 1 && gaps_.front().begin_offset == total_bytes_read_;
}
