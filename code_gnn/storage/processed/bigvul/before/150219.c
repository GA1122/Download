void TabletModeWindowManager::OnWindowVisibilityChanged(aura::Window* window,
                                                        bool visible) {
  if (IsTrackingWindow(window))
    return;

  if (IsContainerWindow(window->parent()) &&
      base::Contains(added_windows_, window) && visible) {
    added_windows_.erase(window);
    window->RemoveObserver(this);
    TrackWindow(window);
    if (IsTrackingWindow(window)) {
      wm::WMEvent event(wm::WM_EVENT_ADDED_TO_WORKSPACE);
      wm::GetWindowState(window)->OnWMEvent(&event);
    }
  }
}
