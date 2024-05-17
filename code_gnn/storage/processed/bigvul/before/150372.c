bool IsPinned(const ash::wm::WindowState* window_state) {
  return window_state->IsPinned() || window_state->IsTrustedPinned();
}
