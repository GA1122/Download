ClientSharedBitmapManager::ClientSharedBitmapManager(
    scoped_refptr<mojom::ThreadSafeSharedBitmapAllocationNotifierPtr>
        shared_bitmap_allocation_notifier)
    : shared_bitmap_allocation_notifier_(
          std::move(shared_bitmap_allocation_notifier)) {}
