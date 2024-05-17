void AXObject::scrollToMakeVisibleWithSubFocus(const IntRect& subfocus) const {
  const AXObject* scrollParent = parentObject() ? parentObject() : this;
  ScrollableArea* scrollableArea = 0;
  while (scrollParent) {
    scrollableArea = scrollParent->getScrollableAreaIfScrollable();
    if (scrollableArea)
      break;
    scrollParent = scrollParent->parentObject();
  }
  if (!scrollParent || !scrollableArea)
    return;

  IntRect objectRect = pixelSnappedIntRect(getBoundsInFrameCoordinates());
  IntSize scrollOffset = scrollableArea->scrollOffsetInt();
  IntRect scrollVisibleRect = scrollableArea->visibleContentRect();

  if (!scrollParent->isWebArea()) {
    objectRect.moveBy(IntPoint(scrollOffset));
    objectRect.moveBy(
        -pixelSnappedIntRect(scrollParent->getBoundsInFrameCoordinates())
             .location());
  }

  int desiredX = computeBestScrollOffset(
      scrollOffset.width(), objectRect.x() + subfocus.x(),
      objectRect.x() + subfocus.maxX(), objectRect.x(), objectRect.maxX(), 0,
      scrollVisibleRect.width());
  int desiredY = computeBestScrollOffset(
      scrollOffset.height(), objectRect.y() + subfocus.y(),
      objectRect.y() + subfocus.maxY(), objectRect.y(), objectRect.maxY(), 0,
      scrollVisibleRect.height());

  scrollParent->setScrollOffset(IntPoint(desiredX, desiredY));

  IntRect newSubfocus = subfocus;
  IntRect newElementRect = pixelSnappedIntRect(getBoundsInFrameCoordinates());
  IntRect scrollParentRect =
      pixelSnappedIntRect(scrollParent->getBoundsInFrameCoordinates());
  newSubfocus.move(newElementRect.x(), newElementRect.y());
  newSubfocus.move(-scrollParentRect.x(), -scrollParentRect.y());

  if (scrollParent->parentObject()) {
    scrollParent->scrollToMakeVisibleWithSubFocus(newSubfocus);
  } else {
    axObjectCache().postNotification(const_cast<AXObject*>(this),
                                     AXObjectCacheImpl::AXLocationChanged);
  }
}
