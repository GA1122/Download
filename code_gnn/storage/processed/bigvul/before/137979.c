AXObject* AXLayoutObject::remoteSVGElementHitTest(const IntPoint& point) const {
  AXObject* remote = remoteSVGRootElement();
  if (!remote)
    return 0;

  IntSize offset =
      point - roundedIntPoint(getBoundsInFrameCoordinates().location());
  return remote->accessibilityHitTest(IntPoint(offset));
}
