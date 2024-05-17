Address NormalPageArena::OutOfLineAllocate(size_t allocation_size,
                                           size_t gc_info_index) {
  DCHECK_GT(allocation_size, RemainingAllocationSize());
  DCHECK_GE(allocation_size, kAllocationGranularity);

  if (allocation_size >= kLargeObjectSizeThreshold)
    return AllocateLargeObject(allocation_size, gc_info_index);

  UpdateRemainingAllocationSize();
  Address result = AllocateFromFreeList(allocation_size, gc_info_index);
  if (result)
    return result;

  SetAllocationPoint(nullptr, 0);

  result = LazySweep(allocation_size, gc_info_index);
  if (result)
    return result;

  GetThreadState()->CompleteSweep();

  GetThreadState()->ScheduleGCIfNeeded();

  AllocatePage();

  result = AllocateFromFreeList(allocation_size, gc_info_index);
  CHECK(result);
  return result;
}
