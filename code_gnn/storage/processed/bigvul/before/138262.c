void AXObjectCacheImpl::inlineTextBoxesUpdated(LineLayoutItem lineLayoutItem) {
  if (!inlineTextBoxAccessibilityEnabled())
    return;

  LayoutObject* layoutObject =
      LineLayoutAPIShim::layoutObjectFrom(lineLayoutItem);

  if (AXObject* obj = get(layoutObject)) {
    if (!obj->needsToUpdateChildren()) {
      obj->setNeedsToUpdateChildren();
      postNotification(layoutObject, AXChildrenChanged);
    }
  }
}
