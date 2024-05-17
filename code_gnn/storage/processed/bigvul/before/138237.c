AXObject* AXObjectCacheImpl::getAriaOwnedParent(const AXObject* child) const {
  return objectFromAXID(m_ariaOwnedChildToOwnerMapping.at(child->axObjectID()));
}
