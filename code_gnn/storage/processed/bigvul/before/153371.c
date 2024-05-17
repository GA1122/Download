gfx::Size TabStrip::CalculatePreferredSize() const {
  int needed_tab_width;
  if (touch_layout_ || adjust_layout_) {
    needed_tab_width =
        GetStackableTabWidth() + (2 * kStackedPadding * kMaxStackedCount);
  } else {
    const int pinned_tab_count = GetPinnedTabCount();
    needed_tab_width = pinned_tab_count * TabStyle::GetPinnedWidth();
    const int remaining_tab_count = tab_count() - pinned_tab_count;
    const int min_selected_width = TabStyleViews::GetMinimumActiveWidth();
    const int min_unselected_width = TabStyleViews::GetMinimumInactiveWidth();
    if (remaining_tab_count > 0) {
      needed_tab_width += min_selected_width +
                          ((remaining_tab_count - 1) * min_unselected_width);
    }

    const int overlap = TabStyle::GetTabOverlap();
    if (tab_count() > 1)
      needed_tab_width -= (tab_count() - 1) * overlap;

    const int largest_min_tab_width =
        min_selected_width + 19 * (min_unselected_width - overlap);
    needed_tab_width = std::min(std::max(needed_tab_width, min_selected_width),
                                largest_min_tab_width);
  }
  return gfx::Size(needed_tab_width + TabToNewTabButtonSpacing() +
                       new_tab_button_bounds_.width() + FrameGrabWidth(),
                   GetLayoutConstant(TAB_HEIGHT));
}
