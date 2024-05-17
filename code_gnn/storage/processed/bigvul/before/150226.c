void TabletModeWindowManager::WindowStateDestroyed(aura::Window* window) {
  DCHECK(!window->HasObserver(this));

  auto it = window_state_map_.find(window);
  if (it != window_state_map_.end())
    window_state_map_.erase(it);
}
