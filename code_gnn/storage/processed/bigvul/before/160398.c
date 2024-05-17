void NormalPageArena::setAllocationPoint(Address point, size_t size) {
#if DCHECK_IS_ON()
  if (point) {
    ASSERT(size);
    BasePage* page = pageFromObject(point);
    ASSERT(!page->isLargeObjectPage());
    ASSERT(size <= static_cast<NormalPage*>(page)->payloadSize());
  }
#endif
  if (hasCurrentAllocationArea()) {
    addToFreeList(currentAllocationPoint(), remainingAllocationSize());
  }
  updateRemainingAllocationSize();
  m_currentAllocationPoint = point;
  m_lastRemainingAllocationSize = m_remainingAllocationSize = size;
}
