void TabletModeWindowState::AttachState(
    wm::WindowState* window_state,
    wm::WindowState::State* previous_state) {
  current_state_type_ = previous_state->GetType();

  gfx::Rect restore_bounds = GetRestoreBounds(window_state);
  if (!restore_bounds.IsEmpty()) {
    SetWindowRestoreOverrides(window_state->window(), restore_bounds,
                              window_state->GetShowState());
  }

  if (current_state_type_ != WindowStateType::kMaximized &&
      current_state_type_ != WindowStateType::kMinimized &&
      current_state_type_ != WindowStateType::kFullscreen &&
      current_state_type_ != WindowStateType::kPinned &&
      current_state_type_ != WindowStateType::kTrustedPinned) {
    UpdateWindow(window_state, state_type_on_attach_,
                 animate_bounds_on_attach_);
  }
}
