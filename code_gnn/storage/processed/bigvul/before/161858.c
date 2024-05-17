void DiscardableSharedMemoryManager::DeletedDiscardableSharedMemory(
    int32_t id,
    int client_id) {
  base::AutoLock lock(lock_);

  MemorySegmentMap& client_segments = clients_[client_id];

  MemorySegmentMap::iterator segment_it = client_segments.find(id);
  if (segment_it == client_segments.end()) {
    LOG(ERROR) << "Invalid discardable shared memory ID";
    return;
  }

  size_t bytes_allocated_before_releasing_memory = bytes_allocated_;

  ReleaseMemory(segment_it->second->memory());

  client_segments.erase(segment_it);

  if (bytes_allocated_ != bytes_allocated_before_releasing_memory)
    BytesAllocatedChanged(bytes_allocated_);
}
