void AXObjectCacheImpl::handleScrollPositionChanged(
    LayoutObject* layoutObject) {
  postPlatformNotification(getOrCreate(layoutObject), AXScrollPositionChanged);
}
