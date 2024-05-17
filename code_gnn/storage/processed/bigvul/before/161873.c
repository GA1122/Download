void DiscardableSharedMemoryManager::ReleaseMemory(
    base::DiscardableSharedMemory* memory) {
  lock_.AssertAcquired();

  size_t size = memory->mapped_size();
  DCHECK_GE(bytes_allocated_, size);
  bytes_allocated_ -= size;

  memory->Unmap();
  memory->Close();
}
