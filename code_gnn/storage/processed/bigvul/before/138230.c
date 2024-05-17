AXObject* AXObjectCacheImpl::firstAccessibleObjectFromNode(const Node* node) {
  if (!node)
    return 0;

  AXObject* accessibleObject = getOrCreate(node->layoutObject());
  while (accessibleObject && accessibleObject->accessibilityIsIgnored()) {
    node = NodeTraversal::next(*node);

    while (node && !node->layoutObject())
      node = NodeTraversal::nextSkippingChildren(*node);

    if (!node)
      return 0;

    accessibleObject = getOrCreate(node->layoutObject());
  }

  return accessibleObject;
}
