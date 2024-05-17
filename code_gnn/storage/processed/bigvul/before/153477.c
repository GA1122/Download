bool TabStrip::ShouldPaintTab(const Tab* tab, float scale, SkPath* clip) {
  if (!MaySetClip())
    return true;

  int index = GetModelIndexOfTab(tab);
  if (index == -1)
    return true;   

  int active_index = drag_context_->IsStackingDraggedTabs()
                         ? controller_->GetActiveIndex()
                         : touch_layout_->active_index();
  if (active_index == tab_count())
    active_index--;

  const gfx::Rect& current_bounds = tab_at(index)->bounds();
  if (index < active_index) {
    const Tab* next_tab = tab_at(index + 1);
    const gfx::Rect& next_bounds = next_tab->bounds();
    if (current_bounds.x() == next_bounds.x())
      return false;

    if (current_bounds.x() > next_bounds.x())
      return true;   

    *clip =
        next_tab->tab_style()->GetPath(TabStyle::PathType::kExteriorClip, scale,
                                       false, TabStyle::RenderUnits::kDips);

    clip->offset(SkIntToScalar(next_bounds.x() - current_bounds.x()), 0);
  } else if (index > active_index && index > 0) {
    const Tab* prev_tab = tab_at(index - 1);
    const gfx::Rect& previous_bounds = prev_tab->bounds();
    if (current_bounds.x() == previous_bounds.x())
      return false;

    if (current_bounds.x() < previous_bounds.x())
      return true;   

    *clip =
        prev_tab->tab_style()->GetPath(TabStyle::PathType::kExteriorClip, scale,
                                       false, TabStyle::RenderUnits::kDips);
    clip->offset(SkIntToScalar(previous_bounds.x() - current_bounds.x()), 0);
  }
  return true;
}
