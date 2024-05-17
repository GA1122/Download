void AXNodeObject::textChanged() {
  AXObjectCacheImpl& cache = axObjectCache();
  for (Node* parentNode = getNode(); parentNode;
       parentNode = parentNode->parentNode()) {
    AXObject* parent = cache.get(parentNode);
    if (!parent)
      continue;

    if (parent->isLiveRegion())
      cache.postNotification(parentNode,
                             AXObjectCacheImpl::AXLiveRegionChanged);

    if (parent->isNonNativeTextControl())
      cache.postNotification(parentNode, AXObjectCacheImpl::AXValueChanged);
  }
}
