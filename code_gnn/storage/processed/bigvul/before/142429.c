void ShelfLayoutManager::CompleteGestureDrag(
    const ui::GestureEvent& gesture_in_screen) {
  if (!ShouldChangeVisibilityAfterDrag(gesture_in_screen)) {
    CancelGestureDrag();
    return;
  }

  shelf_widget_->Deactivate();
  shelf_widget_->status_area_widget()->Deactivate();

  gesture_drag_auto_hide_state_ =
      gesture_drag_auto_hide_state_ == SHELF_AUTO_HIDE_SHOWN
          ? SHELF_AUTO_HIDE_HIDDEN
          : SHELF_AUTO_HIDE_SHOWN;

  gesture_drag_status_ = GESTURE_DRAG_COMPLETE_IN_PROGRESS;
  UpdateVisibilityState();
  gesture_drag_status_ = GESTURE_DRAG_NONE;
}
