Address LargeObjectArena::allocateLargeObjectPage(size_t allocationSize,
                                                  size_t gcInfoIndex) {
  ASSERT(!(allocationSize & allocationMask));

  Address result = lazySweep(allocationSize, gcInfoIndex);
  if (result)
    return result;

  getThreadState()->completeSweep();

  getThreadState()->scheduleGCIfNeeded();

  return doAllocateLargeObjectPage(allocationSize, gcInfoIndex);
}
