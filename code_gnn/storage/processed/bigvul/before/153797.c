void GLES2Implementation::RemoveMappedBufferRangeById(GLuint buffer) {
  if (buffer > 0) {
    auto iter = mapped_buffer_range_map_.find(buffer);
    if (iter != mapped_buffer_range_map_.end() && iter->second.shm_memory) {
      mapped_memory_->FreePendingToken(iter->second.shm_memory,
                                       helper_->InsertToken());
      mapped_buffer_range_map_.erase(iter);
    }
  }
}
