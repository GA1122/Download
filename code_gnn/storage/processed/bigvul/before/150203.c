GetCarryOverWindowsInSplitView() {
  base::flat_map<aura::Window*, WindowStateType> windows;
  MruWindowTracker::WindowList mru_windows =
      Shell::Get()->mru_window_tracker()->BuildWindowForCycleList(kAllDesks);
  if (IsCarryOverCandidateForSplitView(mru_windows, 0u)) {
    if (wm::GetWindowState(mru_windows[0])->GetStateType() ==
        WindowStateType::kLeftSnapped) {
      windows.emplace(mru_windows[0], WindowStateType::kLeftSnapped);
      if (IsCarryOverCandidateForSplitView(mru_windows, 1u) &&
          wm::GetWindowState(mru_windows[1])->GetStateType() ==
              WindowStateType::kRightSnapped) {
        windows.emplace(mru_windows[1], WindowStateType::kRightSnapped);
      }
    } else if (wm::GetWindowState(mru_windows[0])->GetStateType() ==
               WindowStateType::kRightSnapped) {
      windows.emplace(mru_windows[0], WindowStateType::kRightSnapped);
      if (IsCarryOverCandidateForSplitView(mru_windows, 1u) &&
          wm::GetWindowState(mru_windows[1])->GetStateType() ==
              WindowStateType::kLeftSnapped) {
        windows.emplace(mru_windows[1], WindowStateType::kLeftSnapped);
      }
    }
  }
  return windows;
}
