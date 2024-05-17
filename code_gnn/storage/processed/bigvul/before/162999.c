bool QuicStreamSequencerBuffer::RetireBlockIfEmpty(size_t block_index) {
  DCHECK(ReadableBytes() == 0 || GetInBlockOffset(total_bytes_read_) == 0)
      << "RetireBlockIfEmpty() should only be called when advancing to next "
      << "block or a gap has been reached.";
  if (Empty()) {
    return RetireBlock(block_index);
  }

  if (GetBlockIndex(gaps_.back().begin_offset - 1) == block_index) {
    return true;
  }

  if (NextBlockToRead() == block_index) {
    Gap first_gap = gaps_.front();
    DCHECK(first_gap.begin_offset == total_bytes_read_);
    bool gap_ends_in_this_block =
        (GetBlockIndex(first_gap.end_offset) == block_index);
    if (gap_ends_in_this_block) {
      return true;
    }
  }
  return RetireBlock(block_index);
}
