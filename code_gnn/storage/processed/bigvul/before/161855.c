void DiscardableSharedMemoryManager::BytesAllocatedChanged(
    size_t new_bytes_allocated) const {
  static crash_reporter::CrashKeyString<24> total_discardable_memory(
      "total-discardable-memory-allocated");
  total_discardable_memory.Set(base::NumberToString(new_bytes_allocated));
}
