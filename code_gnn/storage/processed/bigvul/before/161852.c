void DiscardableSharedMemoryManager::AllocateLockedDiscardableSharedMemory(
    int client_id,
    size_t size,
    int32_t id,
    base::SharedMemoryHandle* shared_memory_handle) {
  base::AutoLock lock(lock_);

  MemorySegmentMap& client_segments = clients_[client_id];
  if (client_segments.find(id) != client_segments.end()) {
    LOG(ERROR) << "Invalid discardable shared memory ID";
    *shared_memory_handle = base::SharedMemoryHandle();
    return;
  }

  size_t limit = 0;
  if (size < memory_limit_)
    limit = memory_limit_ - size;

  if (bytes_allocated_ > limit)
    ReduceMemoryUsageUntilWithinLimit(limit);

  std::unique_ptr<base::DiscardableSharedMemory> memory(
      new base::DiscardableSharedMemory);
  if (!memory->CreateAndMap(size)) {
    *shared_memory_handle = base::SharedMemoryHandle();
    return;
  }

  base::CheckedNumeric<size_t> checked_bytes_allocated = bytes_allocated_;
  checked_bytes_allocated += memory->mapped_size();
  if (!checked_bytes_allocated.IsValid()) {
    *shared_memory_handle = base::SharedMemoryHandle();
    return;
  }

  bytes_allocated_ = checked_bytes_allocated.ValueOrDie();
  BytesAllocatedChanged(bytes_allocated_);

  *shared_memory_handle = base::SharedMemory::DuplicateHandle(memory->handle());
  memory->Close();

  scoped_refptr<MemorySegment> segment(new MemorySegment(std::move(memory)));
  client_segments[id] = segment.get();
  segments_.push_back(segment.get());
  std::push_heap(segments_.begin(), segments_.end(), CompareMemoryUsageTime);

  if (bytes_allocated_ > memory_limit_)
    ScheduleEnforceMemoryPolicy();
}
