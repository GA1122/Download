IntPoint AXObject::minimumScrollOffset() const {
  ScrollableArea* area = getScrollableAreaIfScrollable();
  if (!area)
    return IntPoint();

  return IntPoint(area->minimumScrollOffsetInt().width(),
                  area->minimumScrollOffsetInt().height());
}
