void AXObjectCacheImpl::handleLayoutComplete(LayoutObject* layoutObject) {
  if (!layoutObject)
    return;

  m_modificationCount++;

  if (AXObject* obj = getOrCreate(layoutObject))
    postNotification(obj, AXLayoutComplete);
}
