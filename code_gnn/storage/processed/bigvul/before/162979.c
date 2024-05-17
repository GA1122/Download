void QuicStreamSequencerBuffer::Clear() {
  if (blocks_ != nullptr) {
    for (size_t i = 0; i < blocks_count_; ++i) {
      if (blocks_[i] != nullptr) {
        RetireBlock(i);
      }
    }
  }
  num_bytes_buffered_ = 0;
  gaps_ = std::list<Gap>(
      1, Gap(total_bytes_read_, std::numeric_limits<QuicStreamOffset>::max()));
  frame_arrival_time_map_.clear();
}
