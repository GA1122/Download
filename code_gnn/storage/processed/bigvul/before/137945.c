void AXLayoutObject::handleActiveDescendantChanged() {
  if (!getLayoutObject())
    return;

  AXObject* focusedObject = axObjectCache().focusedObject();
  if (focusedObject == this && supportsActiveDescendant()) {
    axObjectCache().postNotification(
        getLayoutObject(), AXObjectCacheImpl::AXActiveDescendantChanged);
  }
}
