void AXLayoutObject::offsetBoundingBoxForRemoteSVGElement(
    LayoutRect& rect) const {
  for (AXObject* parent = parentObject(); parent;
       parent = parent->parentObject()) {
    if (parent->isAXSVGRoot()) {
      rect.moveBy(
          parent->parentObject()->getBoundsInFrameCoordinates().location());
      break;
    }
  }
}
