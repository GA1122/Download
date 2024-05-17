void GlobalHistogramAllocator::CreateWithSharedMemoryHandle(
    const SharedMemoryHandle& handle,
    size_t size) {
  std::unique_ptr<SharedMemory> shm(
      new SharedMemory(handle,  false));
  if (!shm->Map(size) ||
      !SharedPersistentMemoryAllocator::IsSharedMemoryAcceptable(*shm)) {
    NOTREACHED();
    return;
  }

  Set(WrapUnique(new GlobalHistogramAllocator(
      std::make_unique<SharedPersistentMemoryAllocator>(
          std::move(shm), 0, StringPiece(),  false))));
}
