void ThreadHeap::PoisonEagerArena() {
  RecursiveMutexLocker persistent_lock(
      ProcessHeap::CrossThreadPersistentMutex());
  arenas_[BlinkGC::kEagerSweepArenaIndex]->PoisonArena();
  ProcessHeap::GetCrossThreadPersistentRegion()
      .UnpoisonCrossThreadPersistents();
}
