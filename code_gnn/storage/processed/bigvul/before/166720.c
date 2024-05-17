Address LargeObjectArena::AllocateLargeObjectPage(size_t allocation_size,
                                                  size_t gc_info_index) {
  DCHECK(!(allocation_size & kAllocationMask));

  Address result = LazySweep(allocation_size, gc_info_index);
  if (result)
    return result;

  GetThreadState()->CompleteSweep();

  GetThreadState()->ScheduleGCIfNeeded();

  return DoAllocateLargeObjectPage(allocation_size, gc_info_index);
}
