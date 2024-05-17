void AXObjectCacheImpl::handleClicked(Node* node) {
  if (AXObject* obj = getOrCreate(node))
    postNotification(obj, AXClicked);
}
