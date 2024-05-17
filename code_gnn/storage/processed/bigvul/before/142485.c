bool ShelfLayoutManager::StartGestureDrag(
    const ui::GestureEvent& gesture_in_screen) {
  if (CanStartFullscreenAppListDrag(
          gesture_in_screen.details().scroll_y_hint())) {
    const gfx::Rect shelf_bounds = GetIdealBounds();
    shelf_background_type_before_drag_ = shelf_background_type_;
    gesture_drag_status_ = GESTURE_DRAG_APPLIST_IN_PROGRESS;
    Shell::Get()->app_list_controller()->Show(
        display::Screen::GetScreen()
            ->GetDisplayNearestWindow(shelf_widget_->GetNativeWindow())
            .id(),
        app_list::kSwipeFromShelf, gesture_in_screen.time_stamp());
    Shell::Get()->app_list_controller()->UpdateYPositionAndOpacity(
        shelf_bounds.y(), GetAppListBackgroundOpacityOnShelfOpacity());
    launcher_above_shelf_bottom_amount_ =
        shelf_bounds.bottom() - gesture_in_screen.location().y();
    return true;
  }

  if (ShouldHomeGestureHandleEvent(
          gesture_in_screen.details().scroll_y_hint())) {
    GestureDragStatus previous_drag_status = gesture_drag_status_;
    gesture_drag_status_ = GESTURE_DRAG_APPLIST_IN_PROGRESS;
    HomeLauncherGestureHandler* home_launcher_handler =
        Shell::Get()->app_list_controller()->home_launcher_gesture_handler();
    if (home_launcher_handler->OnPressEvent(
            HomeLauncherGestureHandler::Mode::kSlideUpToShow,
            gesture_in_screen.location())) {
      return true;
    } else {
      gesture_drag_status_ = previous_drag_status;
    }
  }

  if (is_app_list_visible_ && !IsHomeLauncherEnabledInTabletMode())
    return false;

  if (shelf_widget_->IsShowingOverflowBubble())
    return false;

  gesture_drag_status_ = GESTURE_DRAG_IN_PROGRESS;
  gesture_drag_auto_hide_state_ = visibility_state() == SHELF_AUTO_HIDE
                                      ? auto_hide_state()
                                      : SHELF_AUTO_HIDE_SHOWN;
  MaybeUpdateShelfBackground(AnimationChangeType::ANIMATE);
  gesture_drag_amount_ = 0.f;
  return true;
}
