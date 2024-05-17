String AXObjectCacheImpl::computedNameForNode(Node* node) {
  AXObject* obj = getOrCreate(node);
  if (!obj)
    return "";

  return obj->computedName();
}
