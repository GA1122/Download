void ClientControlledShellSurface::OnWindowStateChangeEvent(
    ash::WindowStateType current_state,
    ash::WindowStateType next_state) {
  if (state_changed_callback_ && pending_window_state_ != next_state)
    state_changed_callback_.Run(current_state, next_state);
}
