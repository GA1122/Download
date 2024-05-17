void AXObjectCacheImpl::handleActiveDescendantChanged(Node* node) {
  m_modificationCount++;

  if (AXObject* obj = getOrCreate(node))
    obj->handleActiveDescendantChanged();
}
