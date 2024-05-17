void PaintLayerScrollableArea::ComputeScrollbarExistence(
    bool& needs_horizontal_scrollbar,
    bool& needs_vertical_scrollbar,
    ComputeScrollbarExistenceOption option) const {
  DCHECK(GetLayoutBox()->GetFrame()->GetSettings());
  if (VisualViewportSuppliesScrollbars() ||
      !CanHaveOverflowScrollbars(*GetLayoutBox()) ||
      GetLayoutBox()->GetFrame()->GetSettings()->GetHideScrollbars()) {
    needs_horizontal_scrollbar = false;
    needs_vertical_scrollbar = false;
    return;
  }

  needs_horizontal_scrollbar = GetLayoutBox()->ScrollsOverflowX();
  needs_vertical_scrollbar = GetLayoutBox()->ScrollsOverflowY();

  if (GetLayoutBox()->HasAutoHorizontalScrollbar()) {
    if (option == kForbidAddingAutoBars)
      needs_horizontal_scrollbar &= HasHorizontalScrollbar();
    needs_horizontal_scrollbar &=
        GetLayoutBox()->IsRooted() && HasHorizontalOverflow() &&
        VisibleContentRect(kIncludeScrollbars).Height();
  }

  if (GetLayoutBox()->HasAutoVerticalScrollbar()) {
    if (option == kForbidAddingAutoBars)
      needs_vertical_scrollbar &= HasVerticalScrollbar();
    needs_vertical_scrollbar &= GetLayoutBox()->IsRooted() &&
                                HasVerticalOverflow() &&
                                VisibleContentRect(kIncludeScrollbars).Width();
  }

  if (GetLayoutBox()->IsLayoutView()) {
    ScrollbarMode h_mode;
    ScrollbarMode v_mode;
    ToLayoutView(GetLayoutBox())->CalculateScrollbarModes(h_mode, v_mode);

    if (h_mode == kScrollbarAlwaysOn)
      needs_horizontal_scrollbar = true;
    else if (h_mode == kScrollbarAlwaysOff)
      needs_horizontal_scrollbar = false;
    if (v_mode == kScrollbarAlwaysOn)
      needs_vertical_scrollbar = true;
    else if (v_mode == kScrollbarAlwaysOff)
      needs_vertical_scrollbar = false;
  }
}
