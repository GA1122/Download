void AXObjectCacheImpl::selectionChanged(Node* node) {
  while (node) {
    if (AXObject* obj = get(node)) {
      obj->selectionChanged();
      return;
    }
    node = node->parentNode();
  }
}
