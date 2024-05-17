void DoSplitViewTransition(
    base::flat_map<aura::Window*, WindowStateType> windows,
    int divider_position) {
  if (windows.empty())
    return;

  SplitViewController* split_view_controller =
      Shell::Get()->split_view_controller();
  if (!split_view_controller->InSplitViewMode())
    split_view_controller->InitDividerPositionForTransition(divider_position);

  for (auto& iter : windows) {
    split_view_controller->SnapWindow(
        iter.first, iter.second == WindowStateType::kLeftSnapped
                        ? SplitViewController::LEFT
                        : SplitViewController::RIGHT);
  }

  if (split_view_controller->InClamshellSplitViewMode() &&
      !Shell::Get()->overview_controller()->InOverviewSession()) {
    split_view_controller->EndSplitView();
  }
}
