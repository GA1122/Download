void AXObjectCacheImpl::handleScrolledToAnchor(const Node* anchorNode) {
  if (!anchorNode)
    return;
  AXObject* obj = getOrCreate(anchorNode->layoutObject());
  if (!obj)
    return;
  if (obj->accessibilityIsIgnored())
    obj = obj->parentObjectUnignored();
  postPlatformNotification(obj, AXScrolledToAnchor);
}
