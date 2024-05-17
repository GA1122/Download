const AtomicString& AXObject::containerLiveRegionRelevant() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedLiveRegionRoot ? m_cachedLiveRegionRoot->liveRegionRelevant()
                                : nullAtom;
}
