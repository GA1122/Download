bool ShelfLayoutManager::ProcessGestureEvent(
    const ui::GestureEvent& event_in_screen) {
  SessionController* controller = Shell::Get()->session_controller();
  if (!controller->NumberOfLoggedInUsers() || controller->IsScreenLocked())
    return false;

  if (IsShelfHiddenForFullscreen())
    return false;

  if (event_in_screen.type() == ui::ET_GESTURE_SCROLL_BEGIN)
    return StartGestureDrag(event_in_screen);

  if (gesture_drag_status_ != GESTURE_DRAG_IN_PROGRESS &&
      gesture_drag_status_ != GESTURE_DRAG_APPLIST_IN_PROGRESS) {
    return false;
  }

  if (event_in_screen.type() == ui::ET_GESTURE_SCROLL_UPDATE) {
    UpdateGestureDrag(event_in_screen);
    return true;
  }

  if (event_in_screen.type() == ui::ET_GESTURE_SCROLL_END ||
      event_in_screen.type() == ui::ET_SCROLL_FLING_START) {
    if (gesture_drag_status_ == GESTURE_DRAG_APPLIST_IN_PROGRESS)
      CompleteAppListDrag(event_in_screen);
    else
      CompleteGestureDrag(event_in_screen);
    return true;
  }

  CancelGestureDrag();
  return false;
}
