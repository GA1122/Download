void AXObjectCacheImpl::textChanged(LayoutObject* layoutObject) {
  textChanged(getOrCreate(layoutObject));
}
