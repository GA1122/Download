bool AXObject::containerLiveRegionAtomic() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedLiveRegionRoot && m_cachedLiveRegionRoot->liveRegionAtomic();
}
