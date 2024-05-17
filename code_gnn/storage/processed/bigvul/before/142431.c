float ShelfLayoutManager::ComputeTargetOpacity(const State& state) const {
  if (gesture_drag_status_ == GESTURE_DRAG_IN_PROGRESS ||
      state.visibility_state == SHELF_VISIBLE) {
    return 1.0f;
  }
  return (state.visibility_state == SHELF_AUTO_HIDE &&
          state.auto_hide_state == SHELF_AUTO_HIDE_SHOWN)
             ? 1.0f
             : 0.0f;
}
