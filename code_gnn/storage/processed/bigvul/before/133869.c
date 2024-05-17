bool HFSForkReadStream::Read(uint8_t* buffer,
                             size_t buffer_size,
                             size_t* bytes_read) {
  size_t buffer_space_remaining = buffer_size;
  *bytes_read = 0;

  if (fork_logical_offset_ == fork_.logicalSize)
    return true;

  for (; current_extent_ < arraysize(fork_.extents); ++current_extent_) {
    if (buffer_space_remaining == 0)
      break;

    const HFSPlusExtentDescriptor* extent = &fork_.extents[current_extent_];

    if (extent->startBlock == 0 && extent->blockCount == 0)
      break;

    auto extent_size =
        base::CheckedNumeric<size_t>(extent->blockCount) * hfs_->block_size();
    if (!extent_size.IsValid()) {
      DLOG(ERROR) << "Extent blockCount overflows";
      return false;
    }

    if (!read_current_extent_) {
      hfs_->SeekToBlock(extent->startBlock);
      current_extent_data_.resize(extent_size.ValueOrDie());
      if (!hfs_->stream()->ReadExact(&current_extent_data_[0],
                                     extent_size.ValueOrDie())) {
        DLOG(ERROR) << "Failed to read extent " << current_extent_;
        return false;
      }

      read_current_extent_ = true;
    }

    size_t extent_offset = fork_logical_offset_ % extent_size.ValueOrDie();
    size_t bytes_to_copy =
        std::min(std::min(static_cast<size_t>(fork_.logicalSize) -
                              fork_logical_offset_,
                          extent_size.ValueOrDie() - extent_offset),
                 buffer_space_remaining);

    memcpy(&buffer[buffer_size - buffer_space_remaining],
           &current_extent_data_[extent_offset],
           bytes_to_copy);

    buffer_space_remaining -= bytes_to_copy;
    *bytes_read += bytes_to_copy;
    fork_logical_offset_ += bytes_to_copy;

    if (fork_logical_offset_ == fork_.logicalSize)
      return true;

    if (extent_offset < current_extent_data_.size())
      break;

    read_current_extent_ = false;
  }

  return true;
}
