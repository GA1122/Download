void TabletModeWindowManager::OnActiveUserSessionChanged(
    const AccountId& account_id) {
  SplitViewController* split_view_controller =
      Shell::Get()->split_view_controller();

  split_view_controller->EndSplitView(
      SplitViewController::EndReason::kActiveUserChanged);

  if (accounts_since_entering_tablet_.count(account_id) == 0u) {
    base::flat_map<aura::Window*, WindowStateType> windows_in_splitview =
        GetCarryOverWindowsInSplitView();
    int divider_position =
        CalculateCarryOverDividerPostion(windows_in_splitview);
    DoSplitViewTransition(windows_in_splitview, divider_position);
    accounts_since_entering_tablet_.insert(account_id);
  } else {
    MruWindowTracker::WindowList windows =
        Shell::Get()->mru_window_tracker()->BuildWindowListIgnoreModal(
            kAllDesks);
    for (aura::Window* window : windows) {
      switch (wm::GetWindowState(window)->GetStateType()) {
        case WindowStateType::kLeftSnapped:
          if (split_view_controller->left_window() == nullptr) {
            split_view_controller->SnapWindow(window,
                                              SplitViewController::LEFT);
          }
          break;
        case WindowStateType::kRightSnapped:
          if (split_view_controller->right_window() == nullptr) {
            split_view_controller->SnapWindow(window,
                                              SplitViewController::RIGHT);
          }
          break;
        default:
          break;
      }
      if (split_view_controller->state() == SplitViewState::kBothSnapped)
        break;
    }
  }

  OverviewController* overview_controller = Shell::Get()->overview_controller();
  SplitViewState state = split_view_controller->state();
  if (state == SplitViewState::kLeftSnapped ||
      state == SplitViewState::kRightSnapped) {
    overview_controller->StartOverview();
  } else {
    overview_controller->EndOverview();
  }
}
