void AXObjectCacheImpl::removeAXID(AXObject* object) {
  if (!object)
    return;

  AXID objID = object->axObjectID();
  if (!objID)
    return;
  DCHECK(!HashTraits<AXID>::isDeletedValue(objID));
  DCHECK(m_idsInUse.contains(objID));
  object->setAXObjectID(0);
  m_idsInUse.erase(objID);

  if (m_ariaOwnerToChildrenMapping.contains(objID)) {
    Vector<AXID> childAXIDs = m_ariaOwnerToChildrenMapping.at(objID);
    for (size_t i = 0; i < childAXIDs.size(); ++i)
      m_ariaOwnedChildToOwnerMapping.erase(childAXIDs[i]);
    m_ariaOwnerToChildrenMapping.erase(objID);
  }
  m_ariaOwnedChildToOwnerMapping.erase(objID);
  m_ariaOwnedChildToRealParentMapping.erase(objID);
  m_ariaOwnerToIdsMapping.erase(objID);
}
