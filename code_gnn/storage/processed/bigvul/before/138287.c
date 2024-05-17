void AXObjectCacheImpl::textChanged(Node* node) {
  textChanged(getOrCreate(node));
}
