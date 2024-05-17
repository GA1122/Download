QuicStreamSequencerBuffer::QuicStreamSequencerBuffer(size_t max_capacity_bytes)
    : max_buffer_capacity_bytes_(max_capacity_bytes),
      blocks_count_(
          ceil(static_cast<double>(max_capacity_bytes) / kBlockSizeBytes)),
      total_bytes_read_(0),
      blocks_(nullptr),
      destruction_indicator_(123456) {
  CHECK_GT(blocks_count_, 1u)
      << "blocks_count_ = " << blocks_count_
      << ", max_buffer_capacity_bytes_ = " << max_buffer_capacity_bytes_;
  Clear();
}
