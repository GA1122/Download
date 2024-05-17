bool CanHandleUnpin() {
  wm::WindowState* window_state = wm::GetActiveWindowState();
  return window_state && window_state->IsPinned();
}
