void AXObjectCacheImpl::onTouchAccessibilityHover(const IntPoint& location) {
  AXObject* hit = root()->accessibilityHitTest(location);
  if (hit) {
    if (hit->getLayoutObject() && hit->getLayoutObject()->isLayoutPart())
      return;

    postPlatformNotification(hit, AXHover);
  }
}
