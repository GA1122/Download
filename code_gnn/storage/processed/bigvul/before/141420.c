bool PaintLayerScrollableArea::HasHorizontalOverflow() const {
  LayoutUnit client_width =
      LayoutContentRect(kIncludeScrollbars).Width() -
      VerticalScrollbarWidth(kIgnorePlatformAndCSSOverlayScrollbarSize);
  if (NeedsRelayout() && !HadVerticalScrollbarBeforeRelayout())
    client_width += VerticalScrollbarWidth();
  LayoutUnit scroll_width(ScrollWidth());
  LayoutUnit box_x = GetLayoutBox()->Location().X();
  return SnapSizeToPixel(scroll_width, box_x) >
         SnapSizeToPixel(client_width, box_x);
}
