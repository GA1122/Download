void AXObjectCacheImpl::childrenChanged(LayoutObject* layoutObject) {
  childrenChanged(get(layoutObject));
}
