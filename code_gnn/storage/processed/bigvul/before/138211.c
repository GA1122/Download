void AXObject::updateCachedAttributeValuesIfNeeded() const {
  if (isDetached())
    return;

  AXObjectCacheImpl& cache = axObjectCache();

  if (cache.modificationCount() == m_lastModificationCount)
    return;

  m_lastModificationCount = cache.modificationCount();
  m_cachedBackgroundColor = computeBackgroundColor();
  m_cachedIsInertOrAriaHidden = computeIsInertOrAriaHidden();
  m_cachedIsDescendantOfLeafNode = (leafNodeAncestor() != 0);
  m_cachedIsDescendantOfDisabledNode = (disabledAncestor() != 0);
  m_cachedHasInheritedPresentationalRole =
      (inheritsPresentationalRoleFrom() != 0);
  m_cachedIsPresentationalChild =
      (ancestorForWhichThisIsAPresentationalChild() != 0);
  m_cachedIsIgnored = computeAccessibilityIsIgnored();
  m_cachedLiveRegionRoot =
      isLiveRegion()
          ? const_cast<AXObject*>(this)
          : (parentObjectIfExists() ? parentObjectIfExists()->liveRegionRoot()
                                    : 0);
  m_cachedAncestorExposesActiveDescendant =
      computeAncestorExposesActiveDescendant();
}
