bool QuicStreamSequencerBuffer::GetReadableRegion(iovec* iov,
                                                  QuicTime* timestamp) const {
  CHECK_EQ(destruction_indicator_, 123456) << "This object has been destructed";

  if (ReadableBytes() == 0) {
    iov[0].iov_base = nullptr;
    iov[0].iov_len = 0;
    return false;
  }

  size_t start_block_idx = NextBlockToRead();
  iov->iov_base = blocks_[start_block_idx]->buffer + ReadOffset();
  size_t readable_bytes_in_block = std::min<size_t>(
      GetBlockCapacity(start_block_idx) - ReadOffset(), ReadableBytes());
  size_t region_len = 0;
  auto iter = frame_arrival_time_map_.begin();
  *timestamp = iter->second.timestamp;
  QUIC_DVLOG(1) << "Readable bytes in block: " << readable_bytes_in_block;
  for (; iter != frame_arrival_time_map_.end() &&
         region_len + iter->second.length <= readable_bytes_in_block;
       ++iter) {
    if (iter->second.timestamp != *timestamp) {
      QUIC_DVLOG(1) << "Meet frame with different timestamp.";
      break;
    }
    region_len += iter->second.length;
    QUIC_DVLOG(1) << "Added bytes to region: " << iter->second.length;
  }
  if (iter == frame_arrival_time_map_.end() ||
      iter->second.timestamp == *timestamp) {
    QUIC_DVLOG(1) << "Got all readable bytes in first block.";
    region_len = readable_bytes_in_block;
  }
  iov->iov_len = region_len;
  return true;
}
