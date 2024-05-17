AXObject* AXLayoutObject::elementAccessibilityHitTest(
    const IntPoint& point) const {
  if (isSVGImage())
    return remoteSVGElementHitTest(point);

  return AXObject::elementAccessibilityHitTest(point);
}
