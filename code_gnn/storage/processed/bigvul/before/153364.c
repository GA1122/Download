void TabStrip::AnimateToIdealBounds() {
  for (int i = 0; i < tab_count(); ++i) {
    Tab* tab = tab_at(i);
    if (tab->dragging() && !bounds_animator_.IsAnimating(tab))
      continue;

    const gfx::Rect& target_bounds = ideal_bounds(i);
    if (bounds_animator_.GetTargetBounds(tab) == target_bounds)
      continue;

    bounds_animator_.AnimateViewTo(
        tab, target_bounds,
        tab->dragging() ? nullptr
                        : std::make_unique<TabAnimationDelegate>(this, tab));
  }

  if (bounds_animator_.GetTargetBounds(new_tab_button_) !=
      new_tab_button_bounds_)
    bounds_animator_.AnimateViewTo(new_tab_button_, new_tab_button_bounds_);
}
