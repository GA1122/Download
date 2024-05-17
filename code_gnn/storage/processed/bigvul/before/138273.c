void AXObjectCacheImpl::postNotification(Node* node,
                                         AXNotification notification) {
  if (!node)
    return;
  postNotification(get(node), notification);
}
