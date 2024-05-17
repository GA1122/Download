Address NormalPageArena::outOfLineAllocate(size_t allocationSize,
                                           size_t gcInfoIndex) {
  ASSERT(allocationSize > remainingAllocationSize());
  ASSERT(allocationSize >= allocationGranularity);

  if (allocationSize >= largeObjectSizeThreshold)
    return allocateLargeObject(allocationSize, gcInfoIndex);

  updateRemainingAllocationSize();
  Address result = allocateFromFreeList(allocationSize, gcInfoIndex);
  if (result)
    return result;

  setAllocationPoint(nullptr, 0);

  result = lazySweep(allocationSize, gcInfoIndex);
  if (result)
    return result;

  if (coalesce()) {
    result = allocateFromFreeList(allocationSize, gcInfoIndex);
    if (result)
      return result;
  }

  getThreadState()->completeSweep();

  getThreadState()->scheduleGCIfNeeded();

  allocatePage();

  result = allocateFromFreeList(allocationSize, gcInfoIndex);
  RELEASE_ASSERT(result);
  return result;
}
