IntPoint AXObject::maximumScrollOffset() const {
  ScrollableArea* area = getScrollableAreaIfScrollable();
  if (!area)
    return IntPoint();

  return IntPoint(area->maximumScrollOffsetInt().width(),
                  area->maximumScrollOffsetInt().height());
}
