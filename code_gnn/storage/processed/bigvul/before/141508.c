bool PaintLayerScrollableArea::TryRemovingAutoScrollbars(
    const bool& needs_horizontal_scrollbar,
    const bool& needs_vertical_scrollbar) {
  DCHECK(!in_overflow_relayout_);

  if (!needs_horizontal_scrollbar && !needs_vertical_scrollbar)
    return false;

  if (GetLayoutBox()->IsLayoutView()) {
    ScrollbarMode h_mode;
    ScrollbarMode v_mode;
    ToLayoutView(GetLayoutBox())->CalculateScrollbarModes(h_mode, v_mode);
    if (h_mode != kScrollbarAuto || v_mode != kScrollbarAuto)
      return false;

    IntSize visible_size_with_scrollbars =
        VisibleContentRect(kIncludeScrollbars).Size();
    if (ScrollWidth() <= visible_size_with_scrollbars.Width() &&
        ScrollHeight() <= visible_size_with_scrollbars.Height()) {
      return true;
    }
  } else {
    if (!GetLayoutBox()->HasAutoVerticalScrollbar() ||
        !GetLayoutBox()->HasAutoHorizontalScrollbar())
      return false;

    LayoutSize client_size_with_scrollbars =
        LayoutContentRect(kIncludeScrollbars).Size();
    if (ScrollWidth() <= client_size_with_scrollbars.Width() &&
        ScrollHeight() <= client_size_with_scrollbars.Height()) {
      return true;
    }
  }

  return false;
}
