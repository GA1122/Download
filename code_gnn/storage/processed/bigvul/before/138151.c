AXObject* AXObject::elementAccessibilityHitTest(const IntPoint& point) const {
  for (const auto& child : m_children) {
    if (child->isMockObject() &&
        child->getBoundsInFrameCoordinates().contains(point))
      return child->elementAccessibilityHitTest(point);
  }

  return const_cast<AXObject*>(this);
}
