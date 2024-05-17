void NormalPageArena::clearFreeLists() {
  setAllocationPoint(nullptr, 0);
  m_freeList.clear();
}
