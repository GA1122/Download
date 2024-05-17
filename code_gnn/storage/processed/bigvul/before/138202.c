void AXObject::setSequentialFocusNavigationStartingPoint() {
  if (parentObject())
    parentObject()->setSequentialFocusNavigationStartingPoint();
}
