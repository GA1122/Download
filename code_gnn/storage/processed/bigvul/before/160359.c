void FreeList::clear() {
  m_biggestFreeListIndex = 0;
  for (size_t i = 0; i < blinkPageSizeLog2; ++i)
    m_freeLists[i] = nullptr;
}
