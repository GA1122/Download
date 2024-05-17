scoped_refptr<base::RefCountedBytes> GetDataFromMojoHandle(
    mojo::ScopedSharedBufferHandle handle) {
  std::unique_ptr<base::SharedMemory> shm =
      GetShmFromMojoHandle(std::move(handle));
  if (!shm)
    return nullptr;

  return base::MakeRefCounted<base::RefCountedBytes>(
      reinterpret_cast<const unsigned char*>(shm->memory()),
      shm->mapped_size());
}
