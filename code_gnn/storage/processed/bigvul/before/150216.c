void TabletModeWindowManager::OnWindowDestroying(aura::Window* window) {
  if (IsContainerWindow(window)) {
    window->RemoveObserver(this);
    observed_container_windows_.erase(window);
  } else if (base::Contains(added_windows_, window)) {
    added_windows_.erase(window);
    window->RemoveObserver(this);
  } else {
    ForgetWindow(window,  true);
  }
}
