PaintLayerScrollableArea::ConvertFromScrollbarToContainingEmbeddedContentView(
    const Scrollbar& scrollbar,
    const IntRect& scrollbar_rect) const {
  LayoutView* view = GetLayoutBox()->View();
  if (!view)
    return scrollbar_rect;

  IntRect rect = scrollbar_rect;
  rect.Move(ScrollbarOffset(scrollbar));

  return view->GetFrameView()->ConvertFromLayoutObject(*GetLayoutBox(), rect);
}
