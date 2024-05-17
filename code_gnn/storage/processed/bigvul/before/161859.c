  DiscardableMemoryImpl(
      std::unique_ptr<base::DiscardableSharedMemory> shared_memory,
      const base::Closure& deleted_callback)
      : shared_memory_(std::move(shared_memory)),
        deleted_callback_(deleted_callback),
        is_locked_(true) {}
