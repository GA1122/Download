void PaintLayerScrollableArea::UpdateNeedsCompositedScrolling(
    bool layer_has_been_composited) {
  const bool needs_composited_scrolling =
      ComputeNeedsCompositedScrolling(layer_has_been_composited, Layer());

  if (static_cast<bool>(needs_composited_scrolling_) !=
      needs_composited_scrolling) {
    needs_composited_scrolling_ = needs_composited_scrolling;
  }
}
