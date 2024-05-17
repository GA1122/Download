AXObject* AXObjectCacheImpl::get(LayoutObject* layoutObject) {
  if (!layoutObject)
    return 0;

  AXID axID = m_layoutObjectMapping.at(layoutObject);
  DCHECK(!HashTraits<AXID>::isDeletedValue(axID));
  if (!axID)
    return 0;

  return m_objects.at(axID);
}
