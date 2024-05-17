    AllocateLockedDiscardableSharedMemoryForClient(
        int client_id,
        size_t size,
        int32_t id,
        base::SharedMemoryHandle* shared_memory_handle) {
  AllocateLockedDiscardableSharedMemory(client_id, size, id,
                                        shared_memory_handle);
}
