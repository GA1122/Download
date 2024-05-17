bool LauncherView::ShouldHideTooltip(const gfx::Point& cursor_location) {
  gfx::Rect active_bounds;

  for (int i = 0; i < child_count(); ++i) {
    views::View* child = child_at(i);
    if (child == overflow_button_)
      continue;

    gfx::Rect child_bounds = child->GetMirroredBounds();
    active_bounds = active_bounds.Union(child_bounds);
  }

  return !active_bounds.Contains(cursor_location);
}
