IntPoint AXObject::getScrollOffset() const {
  ScrollableArea* area = getScrollableAreaIfScrollable();
  if (!area)
    return IntPoint();

  return IntPoint(area->scrollOffsetInt().width(),
                  area->scrollOffsetInt().height());
}
