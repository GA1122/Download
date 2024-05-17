DiscardableSharedMemoryManager::MemorySegment::MemorySegment(
    std::unique_ptr<base::DiscardableSharedMemory> memory)
    : memory_(std::move(memory)) {}
