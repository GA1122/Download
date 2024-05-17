int CalculateCarryOverDividerPostion(
    base::flat_map<aura::Window*, WindowStateType> windows_in_splitview) {
  aura::Window* left_window = nullptr;
  aura::Window* right_window = nullptr;
  for (auto& iter : windows_in_splitview) {
    if (iter.second == WindowStateType::kLeftSnapped)
      left_window = iter.first;
    else if (iter.second == WindowStateType::kRightSnapped)
      right_window = iter.first;
  }
  if (!left_window && !right_window)
    return -1;

  gfx::Rect work_area =
      display::Screen::GetScreen()
          ->GetDisplayNearestWindow(left_window ? left_window : right_window)
          .work_area();
  gfx::Rect left_window_bounds =
      left_window ? left_window->GetBoundsInScreen() : gfx::Rect();
  gfx::Rect right_window_bounds =
      right_window ? right_window->GetBoundsInScreen() : gfx::Rect();

  switch (GetCurrentScreenOrientation()) {
    case OrientationLockType::kLandscapePrimary:
      return left_window ? left_window_bounds.width()
                         : work_area.width() - right_window_bounds.width();
    case OrientationLockType::kPortraitPrimary:
      return left_window ? left_window_bounds.height()
                         : work_area.height() - right_window_bounds.height();
    case OrientationLockType::kLandscapeSecondary:
      return left_window ? work_area.width() - left_window_bounds.width()
                         : right_window_bounds.width();
    case OrientationLockType::kPortraitSecondary:
      return left_window ? work_area.height() - left_window_bounds.height()
                         : right_window_bounds.height();
    default:
      return Shell::Get()->split_view_controller()->GetDefaultDividerPosition(
          left_window ? left_window : right_window);
  }
}
