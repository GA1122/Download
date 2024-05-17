  InputBuffer(uint32_t id,
              std::unique_ptr<SharedMemoryRegion> shm,
              base::OnceCallback<void(int32_t id)> release_cb)
      : id_(id), shm_(std::move(shm)), release_cb_(std::move(release_cb)) {}
