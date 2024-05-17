void ShelfLayoutManager::CompleteAppListDrag(
    const ui::GestureEvent& gesture_in_screen) {
  if (gesture_drag_status_ == GESTURE_DRAG_NONE)
    return;

  HomeLauncherGestureHandler* home_launcher_handler =
      Shell::Get()->app_list_controller()->home_launcher_gesture_handler();
  DCHECK(home_launcher_handler);
  if (home_launcher_handler->OnReleaseEvent(gesture_in_screen.location())) {
    gesture_drag_status_ = GESTURE_DRAG_NONE;
    return;
  }

  using app_list::AppListViewState;
  AppListViewState app_list_state = AppListViewState::PEEKING;
  if (gesture_in_screen.type() == ui::ET_SCROLL_FLING_START &&
      fabs(gesture_in_screen.details().velocity_y()) >
          kAppListDragVelocityThreshold) {
    app_list_state = gesture_in_screen.details().velocity_y() < 0
                         ? AppListViewState::FULLSCREEN_ALL_APPS
                         : AppListViewState::CLOSED;
  } else {
    if (IsTabletModeEnabled()) {
      app_list_state = launcher_above_shelf_bottom_amount_ >
                               kAppListDragSnapToFullscreenThreshold
                           ? AppListViewState::FULLSCREEN_ALL_APPS
                           : AppListViewState::CLOSED;
    } else {
      if (launcher_above_shelf_bottom_amount_ <=
          kAppListDragSnapToClosedThreshold)
        app_list_state = AppListViewState::CLOSED;
      else if (launcher_above_shelf_bottom_amount_ <=
               kAppListDragSnapToPeekingThreshold)
        app_list_state = AppListViewState::PEEKING;
      else
        app_list_state = AppListViewState::FULLSCREEN_ALL_APPS;
    }
  }

  base::Optional<Shelf::ScopedAutoHideLock> auto_hide_lock;
  if (app_list_state == AppListViewState::CLOSED)
    auto_hide_lock.emplace(shelf_);
  Shell::Get()->app_list_controller()->EndDragFromShelf(app_list_state);

  gesture_drag_status_ = GESTURE_DRAG_NONE;
}
