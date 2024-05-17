bool QuicStreamSequencerBuffer::MarkConsumed(size_t bytes_used) {
  CHECK_EQ(destruction_indicator_, 123456) << "This object has been destructed";

  if (bytes_used > ReadableBytes()) {
    return false;
  }
  size_t bytes_to_consume = bytes_used;
  while (bytes_to_consume > 0) {
    size_t block_idx = NextBlockToRead();
    size_t offset_in_block = ReadOffset();
    size_t bytes_available = std::min<size_t>(
        ReadableBytes(), GetBlockCapacity(block_idx) - offset_in_block);
    size_t bytes_read = std::min<size_t>(bytes_to_consume, bytes_available);
    total_bytes_read_ += bytes_read;
    num_bytes_buffered_ -= bytes_read;
    bytes_to_consume -= bytes_read;
    if (bytes_available == bytes_read) {
      RetireBlockIfEmpty(block_idx);
    }
  }
  if (bytes_used > 0) {
    UpdateFrameArrivalMap(total_bytes_read_);
  }
  return true;
}
