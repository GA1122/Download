bool ShelfLayoutManager::IsSwipingCorrectDirection() {
  switch (shelf_->alignment()) {
    case SHELF_ALIGNMENT_BOTTOM:
    case SHELF_ALIGNMENT_BOTTOM_LOCKED:
    case SHELF_ALIGNMENT_RIGHT:
      if (gesture_drag_auto_hide_state_ == SHELF_AUTO_HIDE_SHOWN)
        return gesture_drag_amount_ > 0;
      return gesture_drag_amount_ < 0;
    case SHELF_ALIGNMENT_LEFT:
      if (gesture_drag_auto_hide_state_ == SHELF_AUTO_HIDE_SHOWN)
        return gesture_drag_amount_ < 0;
      return gesture_drag_amount_ > 0;
  }
  return false;
}
