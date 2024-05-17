void AXObjectCacheImpl::handleAriaRoleChanged(Node* node) {
  if (AXObject* obj = getOrCreate(node)) {
    obj->updateAccessibilityRole();
    m_modificationCount++;
    obj->notifyIfIgnoredValueChanged();
  }
}
