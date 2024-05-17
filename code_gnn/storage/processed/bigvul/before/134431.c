void TabStrip::StackDraggedTabs(int delta) {
  DCHECK(!touch_layout_.get());
  GenerateIdealBounds();
  const int active_index = controller_->GetActiveIndex();
  DCHECK_NE(-1, active_index);
  if (delta < 0) {
    const int adjusted_delta =
        std::min(ideal_bounds(active_index).x() -
                     kStackedPadding * std::min(active_index, kMaxStackedCount),
                 -delta);
    for (int i = 0; i <= active_index; ++i) {
      const int min_x = std::min(i, kMaxStackedCount) * kStackedPadding;
      gfx::Rect new_bounds(ideal_bounds(i));
      new_bounds.set_x(std::max(min_x, new_bounds.x() - adjusted_delta));
      set_ideal_bounds(i, new_bounds);
    }
    const bool is_active_mini = tab_at(active_index)->data().mini;
    const int active_width = ideal_bounds(active_index).width();
    for (int i = active_index + 1; i < tab_count(); ++i) {
      const int max_x = ideal_bounds(active_index).x() +
          (kStackedPadding * std::min(i - active_index, kMaxStackedCount));
      gfx::Rect new_bounds(ideal_bounds(i));
      int new_x = std::max(new_bounds.x() + delta, max_x);
      if (new_x == max_x && !tab_at(i)->data().mini && !is_active_mini &&
          new_bounds.width() != active_width)
        new_x += (active_width - new_bounds.width());
      new_bounds.set_x(new_x);
      set_ideal_bounds(i, new_bounds);
    }
  } else {
    const int last_tab_width = ideal_bounds(tab_count() - 1).width();
    const int last_tab_x = width() - new_tab_button_width() - last_tab_width;
    if (active_index == tab_count() - 1 &&
        ideal_bounds(tab_count() - 1).x() == last_tab_x)
      return;
    const int adjusted_delta =
        std::min(last_tab_x -
                     kStackedPadding * std::min(tab_count() - active_index - 1,
                                                kMaxStackedCount) -
                     ideal_bounds(active_index).x(),
                 delta);
    for (int last_index = tab_count() - 1, i = last_index; i >= active_index;
         --i) {
      const int max_x = last_tab_x -
          std::min(tab_count() - i - 1, kMaxStackedCount) * kStackedPadding;
      gfx::Rect new_bounds(ideal_bounds(i));
      int new_x = std::min(max_x, new_bounds.x() + adjusted_delta);
      if (new_x == max_x && !tab_at(i)->data().mini &&
          new_bounds.width() != last_tab_width)
        new_x += (last_tab_width - new_bounds.width());
      new_bounds.set_x(new_x);
      set_ideal_bounds(i, new_bounds);
    }
    for (int i = active_index - 1; i >= 0; --i) {
      const int min_x = ideal_bounds(active_index).x() -
          std::min(active_index - i, kMaxStackedCount) * kStackedPadding;
      gfx::Rect new_bounds(ideal_bounds(i));
      new_bounds.set_x(std::min(min_x, new_bounds.x() + delta));
      set_ideal_bounds(i, new_bounds);
    }
    if (ideal_bounds(tab_count() - 1).right() >= newtab_button_->x())
      newtab_button_->SetVisible(false);
  }
  views::ViewModelUtils::SetViewBoundsToIdealBounds(tabs_);
  SchedulePaint();
}
