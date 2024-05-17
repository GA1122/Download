gfx::Rect ShelfLayoutManager::CalculateTargetBounds(
    const State& state,
    TargetBounds* target_bounds) const {
  const int shelf_size = ShelfConstants::shelf_size();
  int shelf_in_screen_portion = shelf_size;

  if (state.visibility_state == SHELF_AUTO_HIDE &&
      state.auto_hide_state == SHELF_AUTO_HIDE_HIDDEN) {
    shelf_in_screen_portion = kHiddenShelfInScreenPortion;
  } else if (state.visibility_state == SHELF_HIDDEN || IsKeyboardShown()) {
    shelf_in_screen_portion = 0;
  }

  aura::Window* shelf_window = shelf_widget_->GetNativeWindow();
  gfx::Rect available_bounds =
      screen_util::GetDisplayBoundsWithShelf(shelf_window);
  available_bounds.Inset(
      0, accessibility_panel_height_ + docked_magnifier_height_, 0, 0);
  int shelf_width = PrimaryAxisValue(available_bounds.width(), shelf_size);
  int shelf_height = PrimaryAxisValue(shelf_size, available_bounds.height());
  const int shelf_primary_position = SelectValueForShelfAlignment(
      available_bounds.bottom() - shelf_in_screen_portion,
      available_bounds.x() - shelf_size + shelf_in_screen_portion,
      available_bounds.right() - shelf_in_screen_portion);
  gfx::Point shelf_origin = SelectValueForShelfAlignment(
      gfx::Point(available_bounds.x(), shelf_primary_position),
      gfx::Point(shelf_primary_position, available_bounds.y()),
      gfx::Point(shelf_primary_position, available_bounds.y()));

  target_bounds->shelf_bounds = screen_util::SnapBoundsToDisplayEdge(
      gfx::Rect(shelf_origin.x(), shelf_origin.y(), shelf_width, shelf_height),
      shelf_widget_->GetNativeWindow());

  gfx::Size status_size(
      shelf_widget_->status_area_widget()->GetWindowBoundsInScreen().size());
  if (shelf_->IsHorizontalAlignment())
    status_size.set_height(shelf_size);
  else
    status_size.set_width(shelf_size);

  gfx::Point status_origin = SelectValueForShelfAlignment(
      gfx::Point(0, 0),
      gfx::Point(shelf_width - status_size.width(),
                 shelf_height - status_size.height()),
      gfx::Point(0, shelf_height - status_size.height()));
  if (shelf_->IsHorizontalAlignment() && !base::i18n::IsRTL())
    status_origin.set_x(shelf_width - status_size.width());
  target_bounds->status_bounds_in_shelf = gfx::Rect(status_origin, status_size);

  target_bounds->work_area_insets = SelectValueForShelfAlignment(
      gfx::Insets(0, 0, GetWorkAreaInsets(state, shelf_height), 0),
      gfx::Insets(0, GetWorkAreaInsets(state, shelf_width), 0, 0),
      gfx::Insets(0, 0, 0, GetWorkAreaInsets(state, shelf_width)));


  if (!keyboard_displaced_bounds_.IsEmpty()) {
    gfx::Insets keyboard_insets(0, 0, keyboard_displaced_bounds_.height(), 0);
    target_bounds->work_area_insets += keyboard_insets;
  }

  target_bounds->work_area_insets += gfx::Insets(
      accessibility_panel_height_ + docked_magnifier_height_, 0, 0, 0);

  target_bounds->opacity = ComputeTargetOpacity(state);
  target_bounds->status_opacity =
      (state.visibility_state == SHELF_AUTO_HIDE &&
       state.auto_hide_state == SHELF_AUTO_HIDE_HIDDEN &&
       gesture_drag_status_ != GESTURE_DRAG_IN_PROGRESS)
          ? 0.0f
          : target_bounds->opacity;

  if (gesture_drag_status_ == GESTURE_DRAG_IN_PROGRESS)
    UpdateTargetBoundsForGesture(target_bounds);

  target_bounds->shelf_bounds_in_shelf = SelectValueForShelfAlignment(
      gfx::Rect(0, 0, shelf_width - status_size.width(),
                target_bounds->shelf_bounds.height()),
      gfx::Rect(0, 0, target_bounds->shelf_bounds.width(),
                shelf_height - status_size.height()),
      gfx::Rect(0, 0, target_bounds->shelf_bounds.width(),
                shelf_height - status_size.height()));

  available_bounds.Subtract(target_bounds->shelf_bounds);
  available_bounds.Subtract(keyboard_occluded_bounds_);

  aura::Window* root = shelf_window->GetRootWindow();
  ::wm::ConvertRectToScreen(root, &available_bounds);
  return available_bounds;
}
