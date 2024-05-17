void AXObjectCacheImpl::childrenChanged(Node* node) {
  childrenChanged(get(node));
}
