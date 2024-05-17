void AXObjectCacheImpl::remove(Node* node) {
  if (!node)
    return;

  AXID axID = m_nodeObjectMapping.at(node);
  remove(axID);
  m_nodeObjectMapping.erase(node);

  if (node->layoutObject()) {
    remove(node->layoutObject());
    return;
  }
}
