void AXObject::setScrollOffset(const IntPoint& offset) const {
  ScrollableArea* area = getScrollableAreaIfScrollable();
  if (!area)
    return;

  area->setScrollOffset(ScrollOffset(offset.x(), offset.y()),
                        ProgrammaticScroll);
}
