WindowStateType TabletModeWindowManager::GetDesktopWindowStateType(
    aura::Window* window) const {
  auto iter = window_state_map_.find(window);
  return iter == window_state_map_.end()
             ? wm::GetWindowState(window)->GetStateType()
             : iter->second->old_state()->GetType();
}
