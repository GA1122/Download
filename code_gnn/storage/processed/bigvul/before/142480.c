void ShelfLayoutManager::SetState(ShelfVisibilityState visibility_state) {
  State state;
  state.visibility_state = visibility_state;
  state.auto_hide_state = CalculateAutoHideState(visibility_state);

  RootWindowController* controller =
      RootWindowController::ForWindow(shelf_widget_->GetNativeWindow());
  state.window_state = controller ? controller->GetWorkspaceWindowState()
                                  : wm::WORKSPACE_WINDOW_STATE_DEFAULT;
  state.session_state = state_.session_state;
  state.pre_lock_screen_animation_active =
      state_.pre_lock_screen_animation_active;

  bool force_update =
      (gesture_drag_status_ == GESTURE_DRAG_CANCEL_IN_PROGRESS ||
       gesture_drag_status_ == GESTURE_DRAG_COMPLETE_IN_PROGRESS);

  if (!force_update && state_.Equals(state))
    return;   

  for (auto& observer : observers_)
    observer.WillChangeVisibilityState(visibility_state);

  StopAutoHideTimer();

  State old_state = state_;
  state_ = state;

  AnimationChangeType change_type = AnimationChangeType::ANIMATE;
  bool delay_background_change = false;

  if (state.visibility_state == SHELF_VISIBLE &&
      state.window_state == wm::WORKSPACE_WINDOW_STATE_MAXIMIZED &&
      old_state.visibility_state != SHELF_VISIBLE) {
    change_type = AnimationChangeType::IMMEDIATE;
  } else {
    if (state.visibility_state == SHELF_VISIBLE &&
        old_state.visibility_state == SHELF_AUTO_HIDE &&
        old_state.auto_hide_state == SHELF_AUTO_HIDE_HIDDEN) {
      delay_background_change = true;
    }
  }

  if (delay_background_change) {
    if (update_shelf_observer_)
      update_shelf_observer_->Detach();
    update_shelf_observer_ = new UpdateShelfObserver(this);
  } else {
    MaybeUpdateShelfBackground(change_type);
  }

  TargetBounds target_bounds;
  CalculateTargetBoundsAndUpdateWorkArea(&target_bounds);
  UpdateBoundsAndOpacity(
      target_bounds, true  ,
      delay_background_change ? update_shelf_observer_ : nullptr);

  if ((old_state.visibility_state != state_.visibility_state &&
       state_.visibility_state == SHELF_AUTO_HIDE) ||
      old_state.auto_hide_state != state_.auto_hide_state) {
    for (auto& observer : observers_)
      observer.OnAutoHideStateChanged(state_.auto_hide_state);
  }
}
