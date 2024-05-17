void ThreadHeap::PoisonAllHeaps() {
  RecursiveMutexLocker persistent_lock(
      ProcessHeap::CrossThreadPersistentMutex());
  for (int i = 1; i < BlinkGC::kNumberOfArenas; i++)
    arenas_[i]->PoisonArena();
  ProcessHeap::GetCrossThreadPersistentRegion()
      .UnpoisonCrossThreadPersistents();
}
