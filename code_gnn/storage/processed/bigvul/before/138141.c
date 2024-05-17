bool AXObject::containerLiveRegionBusy() const {
  updateCachedAttributeValuesIfNeeded();
  return m_cachedLiveRegionRoot && m_cachedLiveRegionRoot->liveRegionBusy();
}
