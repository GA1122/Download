void NormalPageArena::MakeConsistentForGC() {
  BaseArena::MakeConsistentForGC();

  SetAllocationPoint(nullptr, 0);
}
