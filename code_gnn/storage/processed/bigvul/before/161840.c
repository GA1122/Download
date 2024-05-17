void DeletedDiscardableSharedMemoryOnIO(
    mojom::DiscardableSharedMemoryManagerPtr* manager_mojo,
    int32_t id) {
  (*manager_mojo)->DeletedDiscardableSharedMemory(id);
}
