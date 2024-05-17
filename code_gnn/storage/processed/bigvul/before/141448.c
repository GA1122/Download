LayoutRect PaintLayerScrollableArea::LayoutContentRect(
    IncludeScrollbarsInRect scrollbar_inclusion) const {
  LayoutSize layer_size(Layer()->Size());
  LayoutUnit border_width = GetLayoutBox()->BorderWidth();
  LayoutUnit border_height = GetLayoutBox()->BorderHeight();
  LayoutUnit horizontal_scrollbar_height, vertical_scrollbar_width;
  if (scrollbar_inclusion == kExcludeScrollbars) {
    horizontal_scrollbar_height = LayoutUnit(
        HorizontalScrollbar() && !HorizontalScrollbar()->IsOverlayScrollbar()
            ? HorizontalScrollbar()->ScrollbarThickness()
            : 0);
    vertical_scrollbar_width = LayoutUnit(
        VerticalScrollbar() && !VerticalScrollbar()->IsOverlayScrollbar()
            ? VerticalScrollbar()->ScrollbarThickness()
            : 0);
  }

  return LayoutRect(
      LayoutPoint(ScrollPosition()),
      LayoutSize(
          layer_size.Width() - border_width - vertical_scrollbar_width,
          layer_size.Height() - border_height - horizontal_scrollbar_height)
          .ExpandedTo(LayoutSize()));
}
