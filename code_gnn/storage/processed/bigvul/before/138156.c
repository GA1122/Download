LayoutRect AXObject::getBoundsInFrameCoordinates() const {
  AXObject* container = nullptr;
  FloatRect bounds;
  SkMatrix44 transform;
  getRelativeBounds(&container, bounds, transform);
  FloatRect computedBounds(0, 0, bounds.width(), bounds.height());
  while (container && container != this) {
    computedBounds.move(bounds.x(), bounds.y());
    if (!container->isWebArea()) {
      computedBounds.move(-container->getScrollOffset().x(),
                          -container->getScrollOffset().y());
    }
    if (!transform.isIdentity()) {
      TransformationMatrix transformationMatrix(transform);
      transformationMatrix.mapRect(computedBounds);
    }
    container->getRelativeBounds(&container, bounds, transform);
  }
  return LayoutRect(computedBounds);
}
