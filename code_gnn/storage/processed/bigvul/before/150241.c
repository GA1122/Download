WindowStateType TabletModeWindowState::GetSnappedWindowStateType(
    wm::WindowState* window_state,
    WindowStateType target_state) {
  DCHECK(target_state == WindowStateType::kLeftSnapped ||
         target_state == WindowStateType::kRightSnapped);
  return CanSnapInSplitview(window_state->window())
             ? target_state
             : GetMaximizedOrCenteredWindowType(window_state);
}
