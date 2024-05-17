void ClientDiscardableSharedMemoryManager::AllocateOnIO(
    size_t size,
    int32_t id,
    base::SharedMemoryHandle* handle,
    base::ScopedClosureRunner closure_runner) {
  (*manager_mojo_)
      ->AllocateLockedDiscardableSharedMemory(
          static_cast<uint32_t>(size), id,
          base::Bind(
              &ClientDiscardableSharedMemoryManager::AllocateCompletedOnIO,
              base::Unretained(this), handle, base::Passed(&closure_runner)));
}
