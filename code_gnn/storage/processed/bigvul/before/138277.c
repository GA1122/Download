void AXObjectCacheImpl::remove(AXID axID) {
  if (!axID)
    return;

  AXObject* obj = m_objects.at(axID);
  if (!obj)
    return;

  obj->detach();
  removeAXID(obj);

  if (!m_objects.take(axID))
    return;

  DCHECK(m_objects.size() >= m_idsInUse.size());
}
