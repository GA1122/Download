bool AXObject::isScrollableContainer() const {
  return !!getScrollableAreaIfScrollable();
}
