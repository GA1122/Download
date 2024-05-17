void ThreadHeap::Compact() {
  if (!Compaction()->IsCompacting())
    return;


  Compaction()->StartThreadCompaction();
  for (int i = BlinkGC::kHashTableArenaIndex; i >= BlinkGC::kVector1ArenaIndex;
       --i)
    static_cast<NormalPageArena*>(arenas_[i])->SweepAndCompact();
  Compaction()->FinishThreadCompaction();
}
