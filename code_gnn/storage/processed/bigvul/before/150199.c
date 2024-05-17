void TabletModeWindowManager::ArrangeWindowsForTabletMode() {
  MruWindowTracker::WindowList split_view_eligible_windows =
      Shell::Get()->mru_window_tracker()->BuildWindowForCycleList(kAllDesks);
  ScopedSkipUserSessionBlockedCheck scoped_skip_user_session_blocked_check;
  MruWindowTracker::WindowList activatable_windows =
      Shell::Get()->mru_window_tracker()->BuildWindowListIgnoreModal(kAllDesks);

  base::flat_map<aura::Window*, WindowStateType> windows_in_splitview =
      GetCarryOverWindowsInSplitView();
  int divider_position = CalculateCarryOverDividerPostion(windows_in_splitview);

  if (windows_in_splitview.empty()) {
    for (auto* window : activatable_windows)
      TrackWindow(window,  true);
    return;
  }

  for (auto* window : activatable_windows) {
    bool snap = false;
    for (auto& iter : windows_in_splitview) {
      if (window == iter.first) {
        snap = true;
        break;
      }
    }
    TrackWindow(window,  true, snap,
                 false);
  }

  DoSplitViewTransition(windows_in_splitview, divider_position);
 }
