void AXObjectCacheImpl::handleAriaExpandedChange(Node* node) {
  if (AXObject* obj = getOrCreate(node))
    obj->handleAriaExpandedChanged();
}
