AXID AXObjectCacheImpl::getOrCreateAXID(AXObject* obj) {
  const AXID existingAXID = obj->axObjectID();
  if (existingAXID) {
    DCHECK(m_idsInUse.contains(existingAXID));
    return existingAXID;
  }

  const AXID newAXID = generateAXID();

  m_idsInUse.insert(newAXID);
  obj->setAXObjectID(newAXID);
  m_objects.set(newAXID, obj);

  return newAXID;
}
