void PaintLayerScrollableArea::SetScrollCornerAndResizerVisualRect(
    const LayoutRect& rect) {
  scroll_corner_and_resizer_visual_rect_ = rect;
  if (LayoutScrollbarPart* scroll_corner = ScrollCorner())
    scroll_corner->GetMutableForPainting().FirstFragment().SetVisualRect(rect);
  if (LayoutScrollbarPart* resizer = Resizer())
    resizer->GetMutableForPainting().FirstFragment().SetVisualRect(rect);
}
