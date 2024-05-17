void AXObjectCacheImpl::remove(LayoutObject* layoutObject) {
  if (!layoutObject)
    return;

  AXID axID = m_layoutObjectMapping.at(layoutObject);
  remove(axID);
  m_layoutObjectMapping.erase(layoutObject);
}
