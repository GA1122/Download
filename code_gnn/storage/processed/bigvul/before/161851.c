DiscardableSharedMemoryManager::AllocateLockedDiscardableMemory(size_t size) {
  DCHECK_NE(size, 0u);

  int32_t new_id = g_next_discardable_shared_memory_id.GetNext();

  base::SharedMemoryHandle handle;
  AllocateLockedDiscardableSharedMemory(kInvalidUniqueClientID, size, new_id,
                                        &handle);
  std::unique_ptr<base::DiscardableSharedMemory> memory(
      new base::DiscardableSharedMemory(handle));
  if (!memory->Map(size))
    base::TerminateBecauseOutOfMemory(size);
  memory->Close();
  return std::make_unique<DiscardableMemoryImpl>(
      std::move(memory),
      base::Bind(
          &DiscardableSharedMemoryManager::DeletedDiscardableSharedMemory,
          base::Unretained(this), new_id, kInvalidUniqueClientID));
}
