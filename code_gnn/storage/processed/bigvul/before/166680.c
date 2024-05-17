void ThreadHeap::CompleteSweep() {
  static_assert(BlinkGC::kEagerSweepArenaIndex == 0,
                "Eagerly swept arenas must be processed first.");
  for (int i = 0; i < BlinkGC::kNumberOfArenas; i++)
    arenas_[i]->CompleteSweep();
}
