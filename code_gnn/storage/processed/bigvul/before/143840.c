  void RegisterSubprocessAllocator(
      int id,
      std::unique_ptr<base::PersistentHistogramAllocator> allocator) {
    provider_.RegisterSubprocessAllocator(id, std::move(allocator));
  }
