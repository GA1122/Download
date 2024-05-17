void TabletModeWindowManager::OnOverviewModeEndingAnimationComplete(
    bool canceled) {
  if (canceled)
    return;

  auto* split_view_controller = Shell::Get()->split_view_controller();

  MruWindowTracker::WindowList windows =
      Shell::Get()->mru_window_tracker()->BuildWindowListIgnoreModal(kAllDesks);
  for (auto* window : windows) {
    if (split_view_controller->left_window() != window &&
        split_view_controller->right_window() != window) {
      MaximizeIfSnapped(window);
    }
  }
}
