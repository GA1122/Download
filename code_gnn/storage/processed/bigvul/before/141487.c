IntSize PaintLayerScrollableArea::ScrollbarOffset(
    const Scrollbar& scrollbar) const {
  if (&scrollbar == VerticalScrollbar()) {
    return IntSize(
        VerticalScrollbarStart(0, Layer()->PixelSnappedSize().Width()),
        GetLayoutBox()->BorderTop().ToInt());
  }

  if (&scrollbar == HorizontalScrollbar()) {
    return IntSize(HorizontalScrollbarStart(0),
                   GetLayoutBox()->BorderTop().ToInt() +
                       VisibleContentRect(kIncludeScrollbars).Height() -
                       HorizontalScrollbar()->ScrollbarThickness());
  }

  NOTREACHED();
  return IntSize();
}
