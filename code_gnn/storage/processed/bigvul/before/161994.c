std::unique_ptr<SharedBitmap> ClientSharedBitmapManager::AllocateSharedBitmap(
    const gfx::Size& size) {
  TRACE_EVENT2("renderer", "ClientSharedBitmapManager::AllocateSharedBitmap",
               "width", size.width(), "height", size.height());
  size_t memory_size;
  if (!SharedBitmap::SizeInBytes(size, &memory_size))
    return nullptr;
  SharedBitmapId id = SharedBitmap::GenerateId();
  std::unique_ptr<base::SharedMemory> memory =
      AllocateSharedMemory(memory_size);
  if (!memory || !memory->Map(memory_size))
    CollectMemoryUsageAndDie(size, memory_size);

  uint32_t sequence_number = NotifyAllocatedSharedBitmap(memory.get(), id);

  memory->Close();

  return std::make_unique<ClientSharedBitmap>(
      shared_bitmap_allocation_notifier_, std::move(memory), id,
      sequence_number);
}
