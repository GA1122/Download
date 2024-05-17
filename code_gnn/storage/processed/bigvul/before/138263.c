bool AXObjectCacheImpl::isAriaOwned(const AXObject* child) const {
  return m_ariaOwnedChildToOwnerMapping.contains(child->axObjectID());
}
