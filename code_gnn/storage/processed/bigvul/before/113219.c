void ShelfLayoutManager::CalculateTargetBounds(
    const State& state,
    TargetBounds* target_bounds) {
  const gfx::Rect& available_bounds(
      status_->GetNativeView()->GetRootWindow()->bounds());
  gfx::Rect status_bounds(status_->GetWindowScreenBounds());
  gfx::Size launcher_size = launcher_ ?
      launcher_widget()->GetContentsView()->GetPreferredSize() : gfx::Size();
  int shelf_size = 0;
  int shelf_width = 0, shelf_height = 0;
  GetShelfSize(&shelf_width, &shelf_height);
  if (state.visibility_state == VISIBLE ||
      (state.visibility_state == AUTO_HIDE &&
       state.auto_hide_state == AUTO_HIDE_SHOWN)) {
    shelf_size = std::max(shelf_width, shelf_height);
  } else if (state.visibility_state == AUTO_HIDE &&
             state.auto_hide_state == AUTO_HIDE_HIDDEN) {
    shelf_size = kAutoHideSize;
  }
  if (alignment_ == SHELF_ALIGNMENT_BOTTOM) {
    int y = available_bounds.bottom();
    y -= shelf_size;
    target_bounds->status_bounds = gfx::Rect(
        base::i18n::IsRTL() ? available_bounds.x() :
        available_bounds.right() - status_bounds.width(),
        y + shelf_height - status_bounds.height(),
        status_bounds.width(), status_bounds.height());
    if (launcher_widget()) {
      target_bounds->launcher_bounds = gfx::Rect(
          available_bounds.x(),
          y + (shelf_height - launcher_size.height()) / 2,
          available_bounds.width(),
          launcher_size.height());
    }
    target_bounds->work_area_insets.Set(
        0, 0, GetWorkAreaSize(state, shelf_height), 0);
  } else {
    int x = (alignment_ == SHELF_ALIGNMENT_LEFT) ?
        available_bounds.x() + shelf_size - shelf_width :
        available_bounds.right() - shelf_size;
    target_bounds->status_bounds = gfx::Rect(
        x, available_bounds.bottom() - status_bounds.height(),
        shelf_width, status_bounds.height());
    if (launcher_widget()) {
      target_bounds->launcher_bounds = gfx::Rect(
          x,
          available_bounds.y(),
          launcher_size.width(),
          available_bounds.height());
    }
    if (alignment_ == SHELF_ALIGNMENT_LEFT) {
      target_bounds->work_area_insets.Set(
          0, GetWorkAreaSize(state, shelf_width), 0, 0);
    } else {
      target_bounds->work_area_insets.Set(
          0, 0, 0, GetWorkAreaSize(state, shelf_width));
    }
  }
  target_bounds->opacity =
      (state.visibility_state == VISIBLE ||
       state.visibility_state == AUTO_HIDE) ? 1.0f : 0.0f;
}
