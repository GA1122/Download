AXObject::AXObject(AXObjectCacheImpl& axObjectCache)
    : m_id(0),
      m_haveChildren(false),
      m_role(UnknownRole),
      m_lastKnownIsIgnoredValue(DefaultBehavior),
      m_explicitContainerID(0),
      m_parent(nullptr),
      m_lastModificationCount(-1),
      m_cachedIsIgnored(false),
      m_cachedIsInertOrAriaHidden(false),
      m_cachedIsDescendantOfLeafNode(false),
      m_cachedIsDescendantOfDisabledNode(false),
      m_cachedHasInheritedPresentationalRole(false),
      m_cachedIsPresentationalChild(false),
      m_cachedAncestorExposesActiveDescendant(false),
      m_cachedLiveRegionRoot(nullptr),
      m_axObjectCache(&axObjectCache) {
  ++s_numberOfLiveAXObjects;
}
