void AXLayoutObject::updateChildrenIfNecessary() {
  if (needsToUpdateChildren())
    clearChildren();

  AXObject::updateChildrenIfNecessary();
}
