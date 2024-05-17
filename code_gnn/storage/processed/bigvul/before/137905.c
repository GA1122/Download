AXObject* AXLayoutObject::accessibilityImageMapHitTest(
    HTMLAreaElement* area,
    const IntPoint& point) const {
  if (!area)
    return 0;

  AXObject* parent = axObjectCache().getOrCreate(area->imageElement());
  if (!parent)
    return 0;

  for (const auto& child : parent->children()) {
    if (child->getBoundsInFrameCoordinates().contains(point))
      return child.get();
  }

  return 0;
}
