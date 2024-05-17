Address LargeObjectArena::DoAllocateLargeObjectPage(size_t allocation_size,
                                                    size_t gc_info_index) {
  size_t large_object_size =
      LargeObjectPage::PageHeaderSize() + allocation_size;
#if defined(ADDRESS_SANITIZER)
  large_object_size += kAllocationGranularity;
#endif

  GetThreadState()->Heap().address_cache()->MarkDirty();
  PageMemory* page_memory = PageMemory::Allocate(
      large_object_size, GetThreadState()->Heap().GetRegionTree());
  Address large_object_address = page_memory->WritableStart();
  Address header_address =
      large_object_address + LargeObjectPage::PageHeaderSize();
#if DCHECK_IS_ON()
  for (size_t i = 0; i < large_object_size; ++i)
    DCHECK(!large_object_address[i]);
#endif
  DCHECK_GT(gc_info_index, 0u);
  LargeObjectPage* large_object = new (large_object_address)
      LargeObjectPage(page_memory, this, allocation_size);
  HeapObjectHeader* header = new (NotNull, header_address) HeapObjectHeader(
      kLargeObjectSizeInHeader, gc_info_index, HeapObjectHeader::kLargePage);
  Address result = header_address + sizeof(*header);
  DCHECK(!(reinterpret_cast<uintptr_t>(result) & kAllocationMask));

  ASAN_POISON_MEMORY_REGION(header, sizeof(*header));
  ASAN_POISON_MEMORY_REGION(large_object->GetAddress() + large_object->size(),
                            kAllocationGranularity);

  large_object->Link(&first_page_);

  GetThreadState()->Heap().HeapStats().IncreaseAllocatedSpace(
      large_object->size());
  GetThreadState()->Heap().HeapStats().IncreaseAllocatedObjectSize(
      large_object->size());
  return result;
}
