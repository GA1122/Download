  void TrackMemoryAllocatedChange(size_t old_size, size_t new_size) {
    gpu_memory_manager_tracking_group_->TrackMemoryAllocatedChange(
        old_size, new_size);
  }
