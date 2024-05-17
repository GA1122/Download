void ShelfLayoutManager::UpdateGestureDrag(
    const ui::GestureEvent& gesture_in_screen) {
  if (ShouldHomeGestureHandleEvent(gesture_in_screen.details().scroll_y())) {
    HomeLauncherGestureHandler* home_launcher_handler =
        Shell::Get()->app_list_controller()->home_launcher_gesture_handler();
    if (home_launcher_handler->OnScrollEvent(
            gesture_in_screen.location(),
            gesture_in_screen.details().scroll_y())) {
      return;
    }
  }

  if (gesture_drag_status_ == GESTURE_DRAG_APPLIST_IN_PROGRESS) {
    if (!shelf_->IsHorizontalAlignment()) {
      Shell::Get()->app_list_controller()->DismissAppList();
      launcher_above_shelf_bottom_amount_ = 0.f;
      gesture_drag_status_ = GESTURE_DRAG_NONE;
      return;
    }
    const gfx::Rect shelf_bounds = GetIdealBounds();
    Shell::Get()->app_list_controller()->UpdateYPositionAndOpacity(
        std::min(gesture_in_screen.location().y(), shelf_bounds.y()),
        GetAppListBackgroundOpacityOnShelfOpacity());
    launcher_above_shelf_bottom_amount_ =
        shelf_bounds.bottom() - gesture_in_screen.location().y();
  } else {
    gesture_drag_amount_ +=
        PrimaryAxisValue(gesture_in_screen.details().scroll_y(),
                         gesture_in_screen.details().scroll_x());
    LayoutShelf();
  }
}
