const AtomicString& AXObject::containerLiveRegionStatus() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedLiveRegionRoot ? m_cachedLiveRegionRoot->liveRegionStatus()
                                : nullAtom;
}
