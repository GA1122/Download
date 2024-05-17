void PaintLayerScrollableArea::ScrollControlWasSetNeedsPaintInvalidation() {
  GetLayoutBox()->SetShouldCheckForPaintInvalidation();
}
