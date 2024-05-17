AXID AXObjectCacheImpl::generateAXID() const {
  static AXID lastUsedID = 0;

  AXID objID = lastUsedID;
  do {
    ++objID;
  } while (!objID || HashTraits<AXID>::isDeletedValue(objID) ||
           m_idsInUse.contains(objID));

  lastUsedID = objID;

  return objID;
}
