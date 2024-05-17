IntRect PaintLayerScrollableArea::RectForHorizontalScrollbar(
    const IntRect& border_box_rect) const {
  if (!HasHorizontalScrollbar())
    return IntRect();

  const IntRect& scroll_corner = ScrollCornerRect();

  return IntRect(
      HorizontalScrollbarStart(border_box_rect.X()),
      border_box_rect.MaxY() - GetLayoutBox()->BorderBottom().ToInt() -
          HorizontalScrollbar()->ScrollbarThickness(),
      border_box_rect.Width() -
          (GetLayoutBox()->BorderLeft() + GetLayoutBox()->BorderRight())
              .ToInt() -
          scroll_corner.Width(),
      HorizontalScrollbar()->ScrollbarThickness());
}
