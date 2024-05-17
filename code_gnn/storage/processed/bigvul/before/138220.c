void AXObjectCacheImpl::childrenChanged(AXObject* obj) {
  if (!obj)
    return;

  obj->childrenChanged();
}
