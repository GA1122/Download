CrossThreadPersistentRegion& ProcessHeap::GetCrossThreadWeakPersistentRegion() {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(CrossThreadPersistentRegion,
                                  persistent_region, ());
  return persistent_region;
}
