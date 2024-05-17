void AXNodeObject::childrenChanged() {
  if (!getNode() && !getLayoutObject())
    return;

  if (ancestorForWhichThisIsAPresentationalChild()) {
    setNeedsToUpdateChildren();
    return;
  }

  axObjectCache().postNotification(this, AXObjectCacheImpl::AXChildrenChanged);

  for (AXObject* parent = this; parent;
       parent = parent->parentObjectIfExists()) {
    parent->setNeedsToUpdateChildren();


    if (parent->isLiveRegion())
      axObjectCache().postNotification(parent,
                                       AXObjectCacheImpl::AXLiveRegionChanged);

    if (isNonNativeTextControl())
      axObjectCache().postNotification(parent,
                                       AXObjectCacheImpl::AXValueChanged);
  }
}
