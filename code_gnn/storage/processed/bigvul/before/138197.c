void AXObject::scrollToMakeVisible() const {
  IntRect objectRect = pixelSnappedIntRect(getBoundsInFrameCoordinates());
  objectRect.setLocation(IntPoint());
  scrollToMakeVisibleWithSubFocus(objectRect);
}
