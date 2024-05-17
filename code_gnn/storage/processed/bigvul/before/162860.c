size_t GLManager::GetSharedMemoryBytesAllocated() const {
  return decoder_->GetContextGroup()
      ->transfer_buffer_manager()
      ->shared_memory_bytes_allocated();
}
