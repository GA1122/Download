void PaintLayerScrollableArea::ContentsResized() {
  ScrollableArea::ContentsResized();
  GetLayoutBox()->SetNeedsPaintPropertyUpdate();
  Layer()->SetNeedsCompositingInputsUpdate();
}
