SharedMemoryHandleProvider::GetNonOwnedSharedMemoryHandleForLegacyIPC() {
  return shared_memory_->handle();
}
