void DiscardableSharedMemoryManager::ClientRemoved(int client_id) {
  base::AutoLock lock(lock_);

  auto it = clients_.find(client_id);
  if (it == clients_.end())
    return;

  size_t bytes_allocated_before_releasing_memory = bytes_allocated_;

  for (auto& segment_it : it->second)
    ReleaseMemory(segment_it.second->memory());

  clients_.erase(it);

  if (bytes_allocated_ != bytes_allocated_before_releasing_memory)
    BytesAllocatedChanged(bytes_allocated_);
}
