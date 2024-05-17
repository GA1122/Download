bool TabletModeWindowManager::IsTrackingWindow(aura::Window* window) {
  return base::Contains(window_state_map_, window);
}
