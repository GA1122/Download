RecursiveMutex& ProcessHeap::CrossThreadPersistentMutex() {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(RecursiveMutex, mutex, ());
  return mutex;
}
