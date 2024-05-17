bool IsWindowMinimized(aura::Window* window) {
  return wm::GetWindowState(window)->IsMinimized();
}
