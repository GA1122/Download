bool TabletModeWindowManager::ShouldHandleWindow(aura::Window* window) {
  DCHECK(window);

  if (window->GetProperty(aura::client::kZOrderingKey) !=
      ui::ZOrderLevel::kNormal) {
    return false;
  }

  if (!wm::GetWindowState(window) ||
      wm::GetWindowState(window)->allow_set_bounds_direct()) {
    return false;
  }

  return window->type() == aura::client::WINDOW_TYPE_NORMAL;
}
