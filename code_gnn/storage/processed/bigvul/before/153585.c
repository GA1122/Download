void GLES2Implementation::ClearMappedBufferRangeMap() {
  for (auto& buffer_range : mapped_buffer_range_map_) {
    if (buffer_range.second.shm_memory) {
      mapped_memory_->FreePendingToken(buffer_range.second.shm_memory,
                                       helper_->InsertToken());
    }
  }
  mapped_buffer_range_map_.clear();
}
