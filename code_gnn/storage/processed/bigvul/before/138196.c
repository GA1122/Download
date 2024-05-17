void AXObject::scrollToGlobalPoint(const IntPoint& globalPoint) const {
  HeapVector<Member<const AXObject>> objects;
  AXObject* parentObject;
  for (parentObject = this->parentObject(); parentObject;
       parentObject = parentObject->parentObject()) {
    if (parentObject->getScrollableAreaIfScrollable())
      objects.push_front(parentObject);
  }
  objects.push_back(this);

  int offsetX = 0, offsetY = 0;
  IntPoint point = globalPoint;
  size_t levels = objects.size() - 1;
  for (size_t i = 0; i < levels; i++) {
    const AXObject* outer = objects[i];
    const AXObject* inner = objects[i + 1];
    ScrollableArea* scrollableArea = outer->getScrollableAreaIfScrollable();

    IntRect innerRect =
        inner->isWebArea()
            ? pixelSnappedIntRect(
                  inner->parentObject()->getBoundsInFrameCoordinates())
            : pixelSnappedIntRect(inner->getBoundsInFrameCoordinates());
    IntRect objectRect = innerRect;
    IntSize scrollOffset = scrollableArea->scrollOffsetInt();

    objectRect.move(offsetX, offsetY);
    if (!outer->isWebArea())
      objectRect.move(scrollOffset.width(), scrollOffset.height());

    int desiredX = computeBestScrollOffset(0, objectRect.x(), objectRect.maxX(),
                                           objectRect.x(), objectRect.maxX(),
                                           point.x(), point.x());
    int desiredY = computeBestScrollOffset(0, objectRect.y(), objectRect.maxY(),
                                           objectRect.y(), objectRect.maxY(),
                                           point.y(), point.y());
    outer->setScrollOffset(IntPoint(desiredX, desiredY));

    if (outer->isWebArea() && !inner->isWebArea()) {
      scrollOffset = scrollableArea->scrollOffsetInt();
      offsetX -= (scrollOffset.width() + point.x());
      offsetY -= (scrollOffset.height() + point.y());
      point.move(scrollOffset.width() - innerRect.width(),
                 scrollOffset.height() - innerRect.y());
    } else if (inner->isWebArea()) {
      offsetX = 0;
      offsetY = 0;
    }
  }

  DCHECK(objects[0]);
  axObjectCache().postNotification(objects[0]->getNode(),
                                   AXObjectCacheImpl::AXLocationChanged);
}
