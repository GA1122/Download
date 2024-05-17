  ClientSharedBitmap(
      scoped_refptr<mojom::ThreadSafeSharedBitmapAllocationNotifierPtr>
          shared_bitmap_allocation_notifier,
      std::unique_ptr<base::SharedMemory> shared_memory_holder,
      const SharedBitmapId& id,
      uint32_t sequence_number)
      : ClientSharedBitmap(std::move(shared_bitmap_allocation_notifier),
                           shared_memory_holder.get(),
                           id,
                           sequence_number) {
    shared_memory_holder_ = std::move(shared_memory_holder);
  }
