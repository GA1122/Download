const AtomicString& AXObjectCacheImpl::computedRoleForNode(Node* node) {
  AXObject* obj = getOrCreate(node);
  if (!obj)
    return AXObject::roleName(UnknownRole);
  return AXObject::roleName(obj->roleValue());
}
