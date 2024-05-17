bool ShelfLayoutManager::ShouldHomeGestureHandleEvent(float scroll_y) const {
  if (!IsVisible())
    return false;

  if (scroll_y >= 0 && gesture_drag_status_ != GESTURE_DRAG_APPLIST_IN_PROGRESS)
    return false;

  return true;
}
