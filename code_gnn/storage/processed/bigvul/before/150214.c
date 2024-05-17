void TabletModeWindowManager::OnSplitViewModeEnded() {
  switch (Shell::Get()->split_view_controller()->end_reason()) {
    case SplitViewController::EndReason::kNormal:
    case SplitViewController::EndReason::kUnsnappableWindowActivated:
    case SplitViewController::EndReason::kPipExpanded:
      break;
    case SplitViewController::EndReason::kHomeLauncherPressed:
    case SplitViewController::EndReason::kActiveUserChanged:
    case SplitViewController::EndReason::kWindowDragStarted:
    case SplitViewController::EndReason::kExitTabletMode:
      return;
  }

  MruWindowTracker::WindowList windows =
      Shell::Get()->mru_window_tracker()->BuildWindowListIgnoreModal(kAllDesks);
  for (auto* window : windows)
    MaximizeIfSnapped(window);
}
