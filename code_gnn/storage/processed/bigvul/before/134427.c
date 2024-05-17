bool TabStrip::ShouldPaintTab(const Tab* tab, gfx::Rect* clip) {
  if (!(touch_layout_.get() || IsStackingDraggedTabs()))
    return true;

  int index = GetModelIndexOfTab(tab);
  if (index == -1)
    return true;   

  int active_index = IsStackingDraggedTabs() ?
      controller_->GetActiveIndex() : touch_layout_->active_index();
  if (active_index == tab_count())
    active_index--;

  if (index < active_index) {
    if (tab_at(index)->x() == tab_at(index + 1)->x())
      return false;

    if (tab_at(index)->x() > tab_at(index + 1)->x())
      return true;   

    clip->SetRect(0, 0, tab_at(index + 1)->x() - tab_at(index)->x() +
                      stacked_tab_left_clip(),
                  tab_at(index)->height());
  } else if (index > active_index && index > 0) {
    const gfx::Rect& tab_bounds(tab_at(index)->bounds());
    const gfx::Rect& previous_tab_bounds(tab_at(index - 1)->bounds());
    if (tab_bounds.x() == previous_tab_bounds.x())
      return false;

    if (tab_bounds.x() < previous_tab_bounds.x())
      return true;   

    if (previous_tab_bounds.right() + tab_h_offset() != tab_bounds.x()) {
      int x = previous_tab_bounds.right() - tab_bounds.x() -
          stacked_tab_right_clip();
      clip->SetRect(x, 0, tab_bounds.width() - x, tab_bounds.height());
    }
  }
  return true;
}
