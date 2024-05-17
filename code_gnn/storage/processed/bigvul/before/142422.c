ShelfAutoHideState ShelfLayoutManager::CalculateAutoHideState(
    ShelfVisibilityState visibility_state) const {
  if (!Shell::Get()->session_controller()->IsActiveUserSessionStarted())
    return SHELF_AUTO_HIDE_HIDDEN;

  if (visibility_state != SHELF_AUTO_HIDE)
    return SHELF_AUTO_HIDE_HIDDEN;

  if (shelf_->auto_hide_lock())
    return state_.auto_hide_state;

  if (shelf_widget_->IsShowingAppList() && !IsHomeLauncherEnabledInTabletMode())
    return SHELF_AUTO_HIDE_SHOWN;

  if (shelf_widget_->status_area_widget() &&
      shelf_widget_->status_area_widget()->ShouldShowShelf()) {
    return SHELF_AUTO_HIDE_SHOWN;
  }

  if (shelf_widget_->IsShowingMenu())
    return SHELF_AUTO_HIDE_SHOWN;

  if (shelf_widget_->IsShowingOverflowBubble())
    return SHELF_AUTO_HIDE_SHOWN;

  if (shelf_widget_->IsActive() ||
      (shelf_widget_->status_area_widget() &&
       shelf_widget_->status_area_widget()->IsActive())) {
    return SHELF_AUTO_HIDE_SHOWN;
  }

  if (!HasVisibleWindow())
    return SHELF_AUTO_HIDE_SHOWN;

  if (IsDraggingWindowFromTopOrCaptionArea())
    return SHELF_AUTO_HIDE_SHOWN;

  if (Shell::Get()->overview_controller() &&
      Shell::Get()->overview_controller()->IsSelecting()) {
    return SHELF_AUTO_HIDE_SHOWN;
  }

  if (gesture_drag_status_ == GESTURE_DRAG_APPLIST_IN_PROGRESS)
    return SHELF_AUTO_HIDE_SHOWN;

  if (gesture_drag_status_ == GESTURE_DRAG_COMPLETE_IN_PROGRESS ||
      gesture_drag_status_ == GESTURE_DRAG_CANCEL_IN_PROGRESS) {
    return gesture_drag_auto_hide_state_;
  }

  if (in_mouse_drag_)
    return SHELF_AUTO_HIDE_HIDDEN;

  gfx::Rect shelf_region = GetVisibleShelfBounds();
  if (shelf_widget_->status_area_widget() &&
      shelf_widget_->status_area_widget()->IsMessageBubbleShown() &&
      IsVisible()) {
    ShelfAlignment alignment = shelf_->alignment();
    shelf_region.Inset(
        alignment == SHELF_ALIGNMENT_RIGHT ? -kNotificationBubbleGapHeight : 0,
        shelf_->IsHorizontalAlignment() ? -kNotificationBubbleGapHeight : 0,
        alignment == SHELF_ALIGNMENT_LEFT ? -kNotificationBubbleGapHeight : 0,
        0);
  }

  gfx::Point cursor_position_in_screen =
      display::Screen::GetScreen()->GetCursorScreenPoint();
  if (shelf_region.Contains(cursor_position_in_screen) &&
      !IsTabletModeEnabled()) {
    return SHELF_AUTO_HIDE_SHOWN;
  }

  if ((state_.auto_hide_state == SHELF_AUTO_HIDE_SHOWN ||
       mouse_over_shelf_when_auto_hide_timer_started_) &&
      GetAutoHideShowShelfRegionInScreen().Contains(
          cursor_position_in_screen)) {
    return SHELF_AUTO_HIDE_SHOWN;
  }

  return SHELF_AUTO_HIDE_HIDDEN;
}
