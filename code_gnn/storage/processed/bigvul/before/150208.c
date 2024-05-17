bool TabletModeWindowManager::IsContainerWindow(aura::Window* window) {
  return base::Contains(observed_container_windows_, window);
}
