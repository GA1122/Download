void DiscardableSharedMemoryManager::ClientDeletedDiscardableSharedMemory(
    int32_t id,
    int client_id) {
  DeletedDiscardableSharedMemory(id, client_id);
}
