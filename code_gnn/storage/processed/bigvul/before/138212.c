void AXObject::updateChildrenIfNecessary() {
  if (!hasChildren())
    addChildren();
}
