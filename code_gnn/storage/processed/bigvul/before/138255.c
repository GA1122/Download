void AXObjectCacheImpl::handleScrollPositionChanged(FrameView* frameView) {
  AXObject* targetAXObject = getOrCreate(frameView->frame().document());
  postPlatformNotification(targetAXObject, AXScrollPositionChanged);
}
