void ShelfLayoutManager::CancelGestureDrag() {
  if (gesture_drag_status_ == GESTURE_DRAG_APPLIST_IN_PROGRESS) {
    HomeLauncherGestureHandler* home_launcher_handler =
        Shell::Get()->app_list_controller()->home_launcher_gesture_handler();
    DCHECK(home_launcher_handler);
    if (home_launcher_handler->IsDragInProgress())
      home_launcher_handler->Cancel();
    else
      Shell::Get()->app_list_controller()->DismissAppList();
  } else {
    gesture_drag_status_ = GESTURE_DRAG_CANCEL_IN_PROGRESS;
    UpdateVisibilityState();
  }
  gesture_drag_status_ = GESTURE_DRAG_NONE;
}
