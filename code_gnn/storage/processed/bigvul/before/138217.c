void AXObjectCacheImpl::checkedStateChanged(Node* node) {
  postNotification(node, AXObjectCacheImpl::AXCheckedStateChanged);
}
