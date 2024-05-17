void PaintLayerScrollableArea::ClearPreviousVisualRects() {
  SetHorizontalScrollbarVisualRect(LayoutRect());
  SetVerticalScrollbarVisualRect(LayoutRect());
  SetScrollCornerAndResizerVisualRect(LayoutRect());
}
